# -*- coding: utf-8 -*-
"""
Created on Sat May 29 17:51:08 2021

@author: luo
"""
import glob
import os
from obspy import read,UTCDateTime,read_events,Stream
from obspy.core.inventory import Inventory, Network, Station, Channel, Site

from obspy.clients.nrl import NRL
import pyasdf
def buildInventory(stadict=None):
    # get the RESP
    #resp = '.'.join(['RESP*',network,station,location,'*'])
    #inv = obspy.read_inventory(resp)\
    inv = Inventory(
            # We'll add networks later.
            networks=[],
            # The source should be the id whoever create the file.
            source="ObsPy-Tutorial")
    # By default this accesses the NRL online. Offline copies of the NRL can
    # also be used instead
    nrl = NRL()
    # The contents of the NRL can be explored interactively in a Python prompt,
    # see API documentation of NRL submodule:
    # http://docs.obspy.org/packages/obspy.clients.nrl.html
    # Here we assume that the end point of data logger and sensor are already
    # known:
    #response = nrl.get_response( # doctest: +SKIP
    #    sensor_keys=['Streckeisen', 'STS-1', '360 seconds'],
    #    datalogger_keys=['REF TEK', 'RT 130 & 130-SMA', '1', '200'])
    response = nrl.get_response( # doctest: +SKIP
        sensor_keys=['Sercel/Mark Products', 'L-22D','5470 Ohms','20000 Ohms'],
        datalogger_keys=['REF TEK', 'RT 130 & 130-SMA', '32', '200'])
    
    # Now tie it all together.
    
    for netcode in stadict:
        net = Network(
            # This is the network code according to the SEED standard.
            code=netcode,
            # A list of stations. We'll add one later.
            stations=[],
            description="Weiyuan seismic station",
            # Start-and end dates are optional.
            start_date=UTCDateTime(2009, 3, 1))
        
        stations = stadict[netcode]
        for staname in stations:
            # 修改台站坐标
            loc = stations[staname]
            sta = Station(
                # This is the station code according to the SEED standard.
                code=staname,
                latitude=loc['stla'],
                longitude=loc['stlo'],
                elevation=loc['stel'],
                creation_date=UTCDateTime(2009, 3, 1),
                site=Site(name="First station"))
            chaz = Channel(code="SHZ",location_code="00",latitude=loc['stla'],longitude=loc['stlo'],elevation=loc['stel'],depth=0.0,azimuth=0.0,dip=-90.0,sample_rate=200)
            chan = Channel(code="SHN",location_code="00",latitude=loc['stla'],longitude=loc['stlo'],elevation=loc['stel'],depth=0.0,azimuth=0.0,dip=  0.0,sample_rate=200)
            chae = Channel(code="SHE",location_code="00",latitude=loc['stla'],longitude=loc['stlo'],elevation=loc['stel'],depth=0.0,azimuth=90.0,dip= 0.0,sample_rate=200)
            chaz.response = response
            chan.response = response
            chae.response = response
            sta.channels.append(chaz)
            sta.channels.append(chan)
            sta.channels.append(chae)
            net.stations.append(sta)
        inv.networks.append(net)
    return inv
# 将波形数据和地震目录保存为ASDF格式
if __name__ == '__main__':
    sacfiles = glob.glob(r'G:\Fedora20171028\SanXia\Events\Original\CCdataoriginal'+os.sep+'20090703*San*')
    #sterams = read(r'G:\程序F\matlabwork\Sanxia\Crossrelation\CCdataoriginal'+os.sep+'200907*San*',format='SAC')
    streams = Stream()
    stalist = []
    stadict = {}
    stadict['S1'] = {}
    for sac in sacfiles:
        print(sac)
        tr = read(sac)[0]
        # 修改头端变量
        if tr.stats.channel == "UD":
            tr.stats.channel = "SHZ"
        if tr.stats.channel == "NS":
            tr.stats.channel = "SHN"
        if tr.stats.channel == "EW":
            tr.stats.channel = "SHE"
        tr.stats.network = 'S1'
        tr.stats.location = '00'
        tr.stats.starttime = tr.stats.starttime + 8*60*60
        sta = tr.stats.station
        stlo = tr.stats.sac.stlo
        stla = tr.stats.sac.stla
        stel = tr.stats.sac.stel
        if sta not in stalist:
            stalist.append(sta)
            stadict['S1'][sta] = {'stlo':float(stlo),'stla':float(stla),'stel':float(stel)/1000.}
        #tr = tr.interpolate(100)
        streams.append(tr)
        
    # 读取地震目录数据
    catalog = read_events('7yue.pha',format='HYPODDPHA')
    # 发震时刻为北京时间，需要修改一下        
    # 获取仪器参数
    inv = buildInventory(stadict=stadict)
    # 保存为asdf数据
    #streams.merge()
    with pyasdf.ASDFDataSet('7yue.hdf5',compression='gzip-3') as ds:
        ds.add_waveforms(streams,tag='raw_recording')
        ds.add_quakeml(catalog)
        ds.add_stationxml(inv)
        