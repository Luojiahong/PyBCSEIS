# -*- coding: utf-8 -*-
"""
Created on Wed Jun 10 19:20:40 2020

@author: luo
"""
import numpy as np
import pandas as pd
from obspy.core import UTCDateTime,Stream
import matplotlib.pyplot as plt
#from obspy.core.event import Event, Origin, Pick,Arrival,Catalog
#from obspy.core.event import base
import glob

from obspy.core.event import Event, Origin, Magnitude, Comment, Catalog
from obspy.core.event import EventDescription, CreationInfo, OriginQuality,QuantityError
from obspy.core.event import Pick, WaveformStreamID, Arrival, Amplitude

import obspy.geodetics as geodetics
#import pyasdf

import os
#import conda
#conda_file_dir = conda.__file__
#conda_dir = conda_file_dir.split('lib')[0]
#proj_lib = os.path.join(os.path.join(conda_dir, 'Library'), 'share')
#os.environ["PROJ_LIB"] = proj_lib

def getArrivalForPick(arrivals, pick):
    """
    searches given arrivals for an arrival that references the given
    pick and returns it (empty Arrival object otherwise).
    """
    for a in arrivals:
        if a.pick_id == pick.resource_id:
            return a
    return None

def mag_predict(streams=None,inv=None):
    # 震级评估
    maglist = []
    for tr in streams:
        tr1 = tr.copy()
        tr1.remove_response(inventory=inv,pre_filt=[0.01,0.05,35,40],water_level=10)
        amp = tr1.data.max()
        dist = tr1.stats.distance
        magest = 1.2*np.log10(amp*1e5) + 1.0*np.log10(dist) + 1.0/dist - 0.5# 2-3:1e5
        #magest = np.log10(amp*1e4) + 1.110*np.log10(dist/100) + 0.00189*(dist - 100) +3.0 #4.5
        # 根据CENC提供的地震目录，从连续波形中截取事件波形，进行人工震相标记，测量振幅并拟合系数
        # 2020.11.4更新 最终拟合系数
        #magest = 0.9488*np.log10(amp*1e3) + 1.0733*np.log10(dist/100.) + 0.0131*(dist-100) + 5.2962
        magest = 0.9131*np.log10(amp*1e3) + 1.0383*np.log10(dist/100.) + 0.0135*(dist-100) + 5.2198
        maglist.append([magest,dist,amp,tr1.stats.station,tr1.stats.channel])
    maglist = pd.DataFrame(data=maglist,columns=['magest','dist','amp','sta','cha'])
    magpred = np.median(maglist.magest)    
    return magpred,maglist

def getREALMagnitude(catalog=None,streams=None,inv=None,asdf=None):
    for evt in catalog:
        otime = evt.origins[0].time
        print(otime)
        if not isinstance(streams,Stream):
            with pyasdf.ASDFDataSet(streams) as ds:
                streams = ds.get_waveforms(network="*",station="*",location='*',channel='*',
                                           starttime=otime -10,
                                           endtime=otime +60,
                                           tag='raw_recording')
        pick_dict = {}
        picks = evt.picks
        for pick in picks:
            ptime = pick.time
            station = pick.waveform_id.station_code
            #network_code = pick.waveform_id.network_code
            phase_hint = pick.phase_hint
            arrival = getArrivalForPick(evt.preferred_origin().arrivals,pick)
            dist = geodetics.degrees2kilometers(arrival.distance)
            pick_dict.setdefault(station,{}).setdefault(phase_hint,{}).setdefault('time',ptime)
            #pick_dict.setdefault(station,{}).setdefault(phase_hint,{}).setdefault('residual',time_residual)
            #pick_dict.setdefault(station,{}).setdefault(phase_hint,{}).setdefault('weight',time_weight)
            pick_dict.setdefault(station,{}).setdefault('distance',dist)
        st_mag = Stream()
        for sta in pick_dict:
            pick = pick_dict[sta]
            try:
                ptime = pick['P']['time']
            except:
                ptime = None
            try:
                stime = pick['S']['time']
            except:
                stime = None
            if ptime is None:
                ptime = otime
            if stime is None:
                stime = ptime + 10
            st = streams.select(station=sta).slice(starttime=otime,endtime=stime+20)
            for tr in st:
                tr.stats.distance = pick['distance']
            st_mag += st
        try:
            mag,mag_df = mag_predict(streams=st_mag,inv=inv)
            #magnitude = Magnitude(mag='%.2f'% mag,magnitude_type='ML')
            #magnitudes = [magnitude]
            if len(evt.magnitudes) >=1:
                evt.magnitudes[0].mag = '{:.2f}'.format(mag)
            else:
                evt.magnitudes.append(Magnitude())
                evt.magnitudes[0].mag = '{:.2f}'.format(mag)
                evt.magnitudes[0].magnitude_type = 'M'
                evt.magnitudes[0].creation_info = CreationInfo(agency_id='L22')
            # 将振幅信息写入到事件中
            
        except:
            pass
        if asdf is not None:
            ds = pyasdf.ASDFDataSet(asdf, compression="gzip-3")
            ds.add_quakeml(evt)
            ds.add_stationxml(inv)
            raw_sac = streams.slice(starttime=otime-5,endtime=otime+55)
            ds.add_waveforms(raw_sac,tag='raw_recording',event_id=evt)
            del ds
    return catalog
def getREALStation(stafile=None):
    #stafile = 'station_wy.dat'
    # lon., lat., network, station, component, elevation (km)
    sta_dict = {}
    with open(stafile,'r') as f:
        for line in f:
            #net,sta,chan,lon,lat,ele = line.split()
            lon,lat,net,sta,chan,ele = line.split()
            sta_dict[sta] = {'lon':float(lon),'lat':float(lat),'ele':float(ele)}
    return sta_dict
def getREALPhase(phasefile=None,stafile=None):
    sta_dict = getREALStation(stafile=stafile)
    picks_dict = {}
    p ={}
    picks_sta = []
    catalog = Catalog()
    arrival_list = []
    with open(phasefile,'r') as f:
        for line in f:
            temp = line.split()
            # the header of the phase
            if len(temp) == 16:
                # num, year, mon, day, time (hh:mm:ss), origin time (relative to ZERO, sec), residual (sec), lat., lon., dep., mag., mag var (uncertainty), number of P picks, 
                # number of S picks, total number of picks, station gap
                num,year,mon,day,time,origin_time,residual,lat,lon,dep,mag,mag_var,numP,numS,totalPS,gap = line.split()
                otime = UTCDateTime(''.join([year,mon,day,time]))
                evt = otime.strftime('%Y%m%d%H%M%S.%f')
                print(evt)
                lon = float(lon)
                lat = float(lat)
                mag = float(mag)
                res = float(residual)
                gap = float(gap)
                dep = float(dep)*1000.0
                picks_dict[evt] = {'header':{},'phase':{}}
                picks_dict[evt]['header'] = {'time':{},'lon':{},'lat':{},'dep':{},'mag':{},'res':{},'gap':{}}
                
                picks_dict[evt]['header']['time'] = otime
                picks_dict[evt]['header']['lon'] = lon
                picks_dict[evt]['header']['lat'] = lat
                picks_dict[evt]['header']['dep'] = dep
                picks_dict[evt]['header']['mag'] = mag
                picks_dict[evt]['header']['res'] = res
                picks_dict[evt]['header']['gap'] = gap
                
                # header of the event
                event = Event()
                evtflag = 1
                event.origins.append(Origin())
                event.magnitudes.append(Magnitude())
                
                event.origins[0].time = otime
                event.origins[0].time_errors = res
                event.origins[0].latitude = lat
                event.origins[0].longitude = lon
                event.origins[0].depth = dep
                event.origins[0].depth_errors = 0
                event.magnitudes[0].mag = 0.
                event.magnitudes[0].magnitude_type = 'M'
                event.magnitudes[0].creation_info = CreationInfo(agency_id='REAL')
                
                event.creation_info = CreationInfo(agency_id='REAL')
                event.preferred_origin_id = event.origins[0].resource_id
                
            else:
                # phase line
                # network, station, phase name, absolute travetime (relative to ZERO, sec), 
                # traveltime relative to event origin time (sec), phase amplitude in micrometer, 
                # individual phase residual (sec), weight, azimuth
                net,sta,phase_hint,tt_abs,tt_rel,phase_amplitude,phase_residual,weight,azimuth,gcarc = line.split()
                if sta not in picks_sta: picks_sta.append(sta)
                # get the station
                stlo,stla,stel = sta_dict[sta]['lon'],sta_dict[sta]['lat'],sta_dict[sta]['ele']
                distance = geodetics.gps2dist_azimuth(lat1=stla,lon1=stlo,lat2=lat,lon2=lon)[0]
                phase_time = otime+float(tt_rel)
                picks_dict.setdefault(evt,{}).setdefault('phase',{}).setdefault(sta,{}).setdefault(phase_hint,phase_time)
                picks_dict.setdefault(evt,{}).setdefault('phase',{}).setdefault(sta,{}).setdefault('dist',distance/1000.0)
                
                # phase
                _wavefrom_id = WaveformStreamID(station_code=sta,channel_code=None,network_code='WY')
                pick = Pick(waveform_id=_wavefrom_id,
                            time = phase_time,
                            phase_hint = phase_hint,
                            evaluation_mode = 'automatic',
                            time_errors = QuantityError(uncertainty=float(phase_residual)),
                            backazimuth = azimuth)
                arrival = Arrival(pick_id=pick.resource_id,
                                  phase=phase_hint,
                                  azimuth=azimuth,
                                  distance=geodetics.kilometer2degrees(distance/1000.),
                                  time_residual=phase_residual,
                                  time_weight=weight)
                event.origins[0].arrivals.append(arrival)
                #arrival_list.append([gcarc,arrival])
                pick.distance = distance/1000.
                pick.weight = float(weight)
                event.picks.append(pick)
            if evtflag == 1:
                catalog.append(event)
                evtflag = 0
    # 对震相按照震中距进行排序
    for evt in catalog:
        arrivals = evt.preferred_origin().arrivals
        evt.origins[0].arrivals = []
        arrival_list = []
        for arrival in arrivals:
            arrival_list.append([arrival.distance,arrival])
        arrival_list = pd.DataFrame(data=arrival_list,columns=['distance','arrival'])
        arrival_list = arrival_list.sort_values(by='distance')
        for arrival in arrival_list['arrival'].values:
            evt.origins[0].arrivals.append(arrival)
    return picks_dict,catalog

def getREALCatalog(catalogfile=None):
    # format:
    # num, year, mon, day, time (hh:mm:ss), origin time (relative to ZERO, sec), residual (sec), lat., lon., dep., mag., mag var (uncertainty), number of P picks, number of S picks, total number of picks, station gap
    catalog = []
    tline = ['time','origin_time','residual','latitude','longitude','depth','magnitude','mag_var','numP','numS','totalPS','gap']
    with open(catalogfile,'r') as f:
        for line in f:
            num,year,mon,day,time,origin_time,residual,lat,lon,dep,mag,mag_var,numP,numS,totalPS,gap = line.split()
            #catalog.append(line.split())
            otime = UTCDateTime(''.join([year,mon,day,time]))
            catalog.append([otime,origin_time,float(residual),float(lat),float(lon),float(dep),float(mag),float(mag_var),int(numP),int(numS),int(totalPS),float(gap)])
    catalog = pd.DataFrame(data=catalog,columns=tline)
    return catalog
def REAL2GPD():
    pass
def catalog2hypoDD(catalog=None,hypoDD='hypoDD.pha'):
    # convert the obspy catalog to HypoDD format
    hyp = open(hypoDD,'w')
    evtid = 0
    for i,event in enumerate(catalog):
        origins = event.origins[0]
        time = origins.time
        time_error = origins.time_errors.uncertainty
        if time_error == None:
            time_error = 0.
        longitude = origins.longitude
        latitude = origins.latitude
        depth = origins.depth/1000.
        
        try:
            magnitude = event.magnitudes[0].mag
        except:
            magnitude = 0.
        
        eh = 0
        ez = 0
        
        #evtid = time.strftime('%m%d%H%M%S')
        #evtname = time.strftime("%Y%m%d%H%M%S")
        #evtid = evtid + 1
        evtid = "%04d%05d" %(time.year,i+1)
        hyp.write("# %4d %02d %02d %02d %02d %4.1f %8.4f %9.4f %5.1f %4.1f %5.1f %5.1f %5.1f %s\n" 
                  %(time.year,time.month,time.day,time.hour,time.minute,time.second+time.microsecond/1e6,
                    latitude,longitude,depth,magnitude,eh,ez,time_error,evtid))
        
        picks = event.picks
        for pick in picks:
            ptime = pick.time
            station = pick.waveform_id.station_code
            network_code = pick.waveform_id.network_code
            phase_hint = pick.phase_hint
            arrival = getArrivalForPick(event.preferred_origin().arrivals,pick)
            try:
                weight = arrival.time_weight
                if weight > 1.0: weight = 1.0
            except:
                weight = 1.0
            #hyp.write("%2s%-5s %9.4f %5.4f %1s\n" %(network_code, station, ptime-time, weight, phase_hint))
            hyp.write("%-5s %9.4f %5.4f %1s\n" %(station, ptime-time, weight, phase_hint))
    hyp.close()
def catalog2nlloc(catalog=None,nlloc=None):
    for event in catalog:
        event.write(event.origins[0].time.strftime('%Y%m%d%H%M%S.%f')+'.nlloc',format='NLLOC_OBS')
    fid = open(nlloc,'w')
    files = sorted(glob.glob('*.nlloc'))
    for file in files:
        with open(file,'r') as f:
            for line in f:
                fid.write(line)
        fid.write('\n')
        os.remove(file)
    fid.close()
def event_plot(event=None,streams=None,save=None):
    origins = event.preferred_origin()
    otime = origins.time
    endtime = np.max([t.time for t in event.picks])
    st_new = streams.slice(starttime=otime-10,endtime=endtime+25)
    
    sta_order = []
    picks = {}
    for arrival in origins.arrivals:
        pick = arrival.pick_id.get_referred_object()
        distance = arrival.distance
        sta = pick.waveform_id.station_code
        phase = arrival.phase
        time = pick.time
        if abs(arrival.time_residual) > 5.0:
            flag = 1
        else:
            flag = 0
        if sta not in sta_order:
            picks[sta] = [(time, phase, distance, flag)]
            sta_order.append(sta)
        else:
            picks[sta].append((time, phase, distance, flag))
    
    fig, ax = plt.subplots(1,1,figsize=(15,15))
    count = 0
    for sta in sta_order:
        st = st_new.select(station=sta)
        st.detrend()
        st.filter(type='bandpass', freqmin=0.5, freqmax=20)
        #distance = picks[sta][0][2]*111.0
        for tr in st:
            ax.plot(np.arange(tr.data.size)*tr.stats.delta - 10.0,
                    tr.data/np.max(tr.data) + count, c='k', lw=1)
            ax.text(endtime-otime+26, count, tr.stats.channel[-1]+'_'+sta)
            for pick, phase, distance, flag in picks[sta]:
                if phase == 'P':
                    color = 'r'
                else:
                    color = 'b'
                tr_slice = tr.slice(starttime=pick, endtime=pick+1.0)
                ax.plot(np.arange(tr_slice.data.size)*tr.stats.delta + (pick - otime),
                        tr_slice.data/np.max(tr.data) + count, c=color, lw=1)
                ax.plot([pick-otime, pick-otime], [count-0.75, count+0.75], c=color)
                ax.plot([0,0],[count-0.75, count+0.75],color='g')
            count += 1
    ax.set_yticks([])
    ax.spines['top'].set_visible(False)
    #ax.spines['bottom'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_visible(False)
    if save != None:
        plt.savefig(save,bbox_inches='tight',dpi=300)
    plt.show()
    plt.close()
def stream_plot_event(event=None,streams=None,save=None):
    plt.figure(figsize=(10,10))
    picks = event.picks
    otime = event.origins[0].time
    
    endtime = np.max([t.time for t in event.picks])
    
    st = streams.copy().slice(starttime=otime-10,endtime=endtime+20)
    
    ocolor = 'k'
    statlist = []
    for pick in picks:
        station = pick.waveform_id.station_code.strip()
        tr = st.select(station=station,component='Z')[0]
        if len(tr.data) == 0:
            tr = st.select(station=station,component='E')[0]
        #tr.filter('bandpass', freqmin=0.5, freqmax=20)
        #tr = tr.detrend('linear')
        #tr.data = tr.data - np.mean(tr.data)
        try:
            arrival = getArrivalForPick(event.origins[0].arrivals,pick)
            distance = geodetics.degrees2kilometers(arrival.distance)
        except:
            distance = pick.distance
        if tr.id not in statlist: 
            statlist.append(tr.id)
            #plt.plot(tr.times(),1*tr.data/max(tr.data)+pick.distance,'gray',label=tr.id)
            plt.plot(tr.times(),5*tr.data/max(tr.data)+distance,'gray',label=tr.id)
        if pick.phase_hint == 'P':
            ocolor = 'r'
        else:
            ocolor = 'b'
        plt.vlines(otime-tr.stats.starttime,distance-5,distance+5,'g')#,label='Pick')
        plt.vlines(pick.time-tr.stats.starttime,distance-5,distance+5,ocolor)#,label='Pick')
        plt.text(tr.times()[-10*100],distance,tr.id)
    plt.ylabel('Epicenter Distance [km]')
    plt.xlabel('Time [s]')
    #plt.xlim([50,150])
    if save != None:
        plt.savefig(save,bbox_inches='tight',dpi=300)
    #plt.show()
    plt.close()
def stream_plot(event=None,ds=None,picked_only=False,save=None):
    st_event = Stream()
    otime = event.preferred_origin().time
    longitude = event.preferred_origin().longitude
    latitude = event.preferred_origin().latitude
    depth = event.preferred_origin().depth
    mag = event.magnitudes[0].mag
    
    t = ('Time:'+str(otime) + 
         ' Lon:'+str(longitude) + 
         ' Lat:'+str(latitude) + 
         ' Dep:'+str(depth) +
         ' Mag:'+str(mag))
    sta_picks = [pick.waveform_id.station_code for pick in event.picks]
    
    sta_name = ds.get_all_coordinates()
    event_list = []
    for sta in sta_name:
        st = ds.get_waveforms(network=sta.split('.')[0],
                              station=sta.split('.')[1],
                              location='00',channel='*',
                              starttime=otime-10,endtime=otime+50,tag='raw_recording')
        st_event += st
        # 计算震中距
        stlo = sta_name[sta]['longitude']
        stla = sta_name[sta]['latitude']
        distance = geodetics.gps2dist_azimuth(lat1=stla,lon1=stlo,lat2=latitude,lon2=longitude)[0]
        distance = distance/1000.
        if sta.split('.')[-1] in sta_picks:
            picked = 1
        else:
            picked = 0
        
        for tr in st:
            event_list.append([distance,sta,tr,picked])
        
    event_list = pd.DataFrame(data=event_list,columns=['distance','station_name','streams','picked'])
    event_list = event_list.sort_values(by='distance',ascending=False)
    
    if picked_only:
        event_list = event_list[event_list['picked'] == 1]
    
    fig, axes = plt.subplots(len(event_list),1,figsize=(10,10),sharex=True)
    for i,ax in enumerate(axes):
        tr = event_list.iloc[i]['streams']
        tr.detrend()
        #tr.filter(type='bandpass', freqmin=0.5, freqmax=20)
        sta = tr.stats.station
        color = 'k'
        if sta  in sta_picks:
            color='r'
        if i == len(event_list)-1:
            ax.plot(tr.times(),tr.data/tr.data.max(),color=color)
            ax.set_yticks([])
            ax.spines['top'].set_visible(False)
            #ax.spines['bottom'].set_visible(False)
            ax.spines['right'].set_visible(False)
            ax.spines['left'].set_visible(False)
        else:
            ax.plot(tr.times(),tr.data/tr.data.max(),color=color)
            ax.set_axis_off()
        
        # plot the arrivals
        if event_list.iloc[i]['picked'] == 1:
            pass
        if i==0:
            ax.set_title(t)
        ax.text(tr.times()[-1], 0, tr.id)
        ax.text(0,0,'{:.2f}'.format(event_list.iloc[i]['distance']),ha='right')
    if save != None:
        plt.savefig(save,bbox_inches='tight',dpi=300)
    #plt.show()
    plt.close()
if __name__ == '__main__':
    picks_dict,catalog = getREALPhase(phasefile='phase_all.txt',stafile='../Data/station.dat')
    catalog.plot(projection='local')
    cat = getREALCatalog(catalogfile='catalog_all.txt')