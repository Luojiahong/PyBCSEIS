# -*- coding: utf-8 -*-
"""
Created on Fri Oct 16 15:12:19 2020

@author: luo
"""

import os
import glob
import shutil 

import PyREAL
from obspy import Stream,read
import pandas as pd

import pyasdf
from distaz import DistAz

# 根据asdf数据中的地震目录和台站记录创建BCSEIS计算目录
def getPick(picks=None,pick_id=None):
    for pick in picks:
        if pick.resource_id == pick_id:
            return pick
    return None
def getStaListInDir2(dirName=None,phaCode=None):
    # getStaListInDir2.pl
    #Script:	getStaListInDir2.pl
    #Purpose:	Get the list of station names in a data directory
    #Version:	2.0
    #Major Changes:	Also check for event origin time when checking for phase arrivals in the SAC header
    #Author:	Wen-xuan(Wayne) Du
    #Date:		January 15, 2003
    #Last Modified:	July 1st, 2003
    staList = []
    sacFiles = glob.glob(dirName+os.sep+'*Z.SAC')
    for sacFile in sacFiles:
        sta,com,sac = sacFile.split(os.sep)[-1].split('.')[0:3]
        tr = read(sacFile)[0]
        if sta != 'filt': # not a filtered SAC file
            good = 1
            flag1 = 0
            flag2 = 0
            if phaCode == 0: # P pick
                if 'a' in tr.stats.sac:
                    flag1 = 1
                if flag1 == 0:
                    good = 0
            elif phaCode == 1: # S pick
                if 't0' in tr.stats.sac:
                    flag2 = 1
                if flag2 == 0:
                    good = 0
            else:
                if 'a' in tr.stats.sac and 't0' in tr.stats.sac:
                    flag1 = 1
                    flag2 = 1
                if flag1 ==0 or flag2 == 0:
                    good = 0
            if 'o' not in tr.stats.sac:
                good = 0
            if good == 1:
                staList.append(sta)
    return staList
def getArrivalForPick(arrivals, pick):
    """
    searches given arrivals for an arrival that references the given
    pick and returns it (empty Arrival object otherwise).
    """
    for a in arrivals:
        if a.pick_id == pick.resource_id:
            return a
    return None
def makeBCSEIS(bcseisdir=None):
    if not os.path.exists(bcseisdir):
        #os.mkdir(bcseisdir)
        os.makedirs(bcseisdir+os.sep+'Stations')
        os.makedirs(bcseisdir+os.sep+'Events'+os.sep+'CCEventInfo')
        os.makedirs(bcseisdir+os.sep+'Events'+os.sep+'Data')
        os.makedirs(bcseisdir+os.sep+'Events'+os.sep+'EventInfo')
        os.makedirs(bcseisdir+os.sep+'Events'+os.sep+'IdDirFiles')
        os.makedirs(bcseisdir+os.sep+'Events'+os.sep+'WaveformCC')
        # copy the exe the the WaveformCC
        shutil.copy('BCSEIS'+os.sep+'bin'+os.sep+'bcseis',bcseisdir+os.sep+'Events'+os.sep+'WaveformCC')
        shutil.copy('BCSEIS'+os.sep+'bin'+os.sep+'setsachdr',bcseisdir+os.sep+'Events'+os.sep+'WaveformCC')
        shutil.copy('BCSEIS'+os.sep+'bin'+os.sep+'getsachdr',bcseisdir+os.sep+'Events'+os.sep+'WaveformCC')
def getStaStat(basedir=None):
    # getStaStat.pl station.list $dataDirlist > sta.err
    dataDir_list = basedir+os.sep+'Events/IdDirFiles/iddir.dat'
    station_list = basedir+os.sep+'Stations/station.dat'
    
    outdir = basedir+os.sep+'Events/Data/StaStat'
    if not os.path.exists(outdir):
        os.mkdir(outdir)
    
    stastat = []
    with open(station_list,'r') as f:
        for line in f:
            staname,stla,stlo,stel = line.split()
            sta_list = outdir+os.sep+staname+'.list'
            sacs = []
            with open(dataDir_list,'r') as catf:
                for catline in catf:
                    evtid,datadir = catline.split()
                    sacfiles = glob.glob(basedir+os.sep+'Events/Data'+os.sep+datadir+os.sep+staname+'*.Z.SAC')
                    if len(sacfiles)==1:
                        tmp = os.sep.join([sacfiles[0].split(os.sep)[-2],sacfiles[0].split(os.sep)[-1]])
                        sacs.append(tmp+'\n')
            with open(sta_list,'w') as staf:
                staf.writelines(sacs)
            
            # 对P,S, P&S进行统计
            fp = open(outdir+os.sep+staname+'.plist','w')
            fs = open(outdir+os.sep+staname+'.slist','w')
            fps = open(outdir+os.sep+staname+'.pslist','w')
            
            count = 0
            good0 = 0
            good1 = 0
            good2 = 0
            with open(sta_list,'r') as staf:
                for sline in staf:
                    count += 1
                    flag1 = 0
                    flag2 = 0
                    sacfile = basedir+os.sep+'Events/Data'+os.sep+sline
                    tr = read(sacfile[:-1])[0]
                    
                    # 对数据进行滤波数据
                    # filtAll.pl
                    tr.detrend('linear')
                    tr.detrend('demean')
                    tr.filter('bandpass',freqmin=1,freqmax=15)
                    fout = basedir+os.sep+'Events/Data'+os.sep+sline.split(os.sep)[0]+os.sep+'filt.'+sline.split(os.sep)[1]
                    tr.write(fout[:-1],format='SAC')
                    
                    # get the P arrivals
                    try:
                        a = tr.stats.sac.a
                        fp.write(sline)
                        good0 += 1
                        flag1 = 1
                    except:
                        pass
                    try:
                        t0 = tr.stats.sac.t0
                        fs.write(sline)
                        good1 += 1
                        flag2 = 1
                    except:
                        pass
                    if (flag1 == 1) & (flag2 == 1):
                        fps.write(sline)
                        good2 += 1
            fp.close()
            fs.close()
            fps.close()
            
            #stastat.setdefault(staname,[]).append(count,good0,good1,good2)
            stastat.append([staname,count,good0,good1,good2])
            print(">>> {} out of {} events have P arrivals and the list is saved in file {}".format(good0,count,staname))
            print(">>> {} out of {} events have S arrivals and the list is saved in file {}".format(good1,count,staname))
            print(">>> {} out of {} events have P&S arrivals and the list is saved in file {}".format(good2,count,staname))
    
    #对最终的结果进行统计
    statFile = outdir+os.sep+'staStat.w' # statistical file about the waveform availability for stations in file $staFile
    statFile0 = outdir+os.sep+'staStat.p' # statistical file about the P arrival availability for stations in file $staFile
    statFile1 = outdir+os.sep+'staStat.s' # statistical file about the S arrival availability for stations
    statFile2 = outdir+os.sep+'staStat.pas' # statistical file about the P&S arrival availability for stations
    
    stastat = pd.DataFrame(data=stastat,columns=['staname','w','p','s','pas'])
    with open(statFile,'w') as f:
        df = stastat.sort_values(by='w',ascending=False)
        for i in range(len(df)):
            staname = df.iloc[i].staname
            f.write("%s %s\n" %(staname,staname+'.list'))
    with open(statFile0,'w') as f:
        df = stastat.sort_values(by='p',ascending=False)
        for i in range(len(df)):
            staname = df.iloc[i].staname
            f.write("%s %s\n" %(staname,staname+'.plist'))
    with open(statFile1,'w') as f:
        df = stastat.sort_values(by='s',ascending=False)
        for i in range(len(df)):
            staname = df.iloc[i].staname
            f.write("%s %s\n" %(staname,staname+'.slist'))
    with open(statFile2,'w') as f:
        df = stastat.sort_values(by='pas',ascending=False)
        for i in range(len(df)):
            staname = df.iloc[i].staname
            f.write("%s %s\n" %(staname,staname+'.pslist'))
def buildEvents(basedir=None,asdf=None):
    """
    将输入的ASDF数据文件进行处理，生成对应的文件，目前只能处理一个asdf
    将数据转换为BCSEIS所需的文件
    """
    # 用于生成Events:
    # Data -> 数据波形文件
    # EventInfo -> events.catalog
    # IdDirFiles -> iddir.dat
    # 同时生成
    # Stations -> station.dat
    # 具体文件格式请阅读BCSEIS使用说明
    with pyasdf.ASDFDataSet(asdf,mode='r') as ds:
        catalogs = ds.events
        # save the hypoDD catalogs to the IdDirFiles
        PyREAL.catalog2hypoDD(catalog=catalogs,hypoDD=basedir+os.sep+'Events/IdDirFiles/hypoDD.pha')
        staout = []
        staline = []
        iddir = []
        events_catalog = []
        for i,event in enumerate(catalogs):
            origin_time = event.origins[0].time
            evtname = origin_time.strftime("%Y%m%d%H%M%S")
            datadir = basedir+os.sep+'Events/Data'+os.sep+evtname
            if not os.path.exists(datadir):
                #os.makedirs(datadir)
                os.mkdir(datadir)
            iddir.append("%4d%05d %s\n" %(origin_time.year,i+1,evtname))
            
            evlo = event.origins[0].longitude
            evla = event.origins[0].latitude
            evdp = event.origins[0].depth/1000.
            mag = event.magnitudes[0].mag
            
            tline = "%04d%05d %s %8.5f %9.5f %7.4f %5.2f\n" %(origin_time.year,i+1,origin_time.strftime("%Y %m %d %H %M %S.%f"),evla,evlo,evdp,mag)
            events_catalog.append(tline)
            
            # 0001 2009 11 14  6  7  1.6000  30.98500 110.78900  6.7000  0.10
            print(evtname)
            pickdict = {}
            for pick in event.picks:
                staname = pick.waveform_id.station_code
                pickdict.setdefault(staname,[]).append(pick.resource_id)
            
            #stalista = list(ds.get_all_coordinates())
            for sta in pickdict:
                picks = pickdict[sta]
                # get the station coordinates
                #station_coordinates = ds.waveforms['S1.'+sta].coordinates
                # fixed by network not only S1
                station_group = next((g for g in ds.waveforms.list() if g.endswith('.' + sta)), None)
                if station_group is None:
                    print(f"Error: not find {sta} in data!")
                    break
                station_coordinates = ds.waveforms[station_group].coordinates
                
                stla = station_coordinates['latitude']
                stlo = station_coordinates['longitude']
                stel = station_coordinates['elevation_in_m']*1000.
                if sta not in staout:
                    staline.append("%-5s %7.4f %8.4f %5.4f\n" %(sta,stla,stlo,stel/1000.))
                    staout.append(sta)
                
                # 计算震中距等
                dist = DistAz(lat1=stla,lon1=stlo,lat2=float(evla),lon2=float(evlo))
                distance = dist.degreesToKilometers(dist.getDelta())
                #asdfname = origin_time.strftime("%Y%m%d")
                #asdfname = 'E:\Python\PhaseNet\ASDFDataset'+os.sep+origin_time.strftime("%Y%m%d")+'.hdf5'
                #with pyasdf.ASDFDataSet(asdfname,mode='r') as evtds:
                #    st_new = evtds.get_waveforms(network='*',station=sta,location='*',channel='*',starttime=origin_time,endtime=origin_time+60,tag='raw_recording')
                #    st_new.interpolate(100)
                st_new = ds.get_waveforms(network='*',station=sta,location='*',channel='*',starttime=origin_time,endtime=origin_time+60,tag='raw_recording')
                for tr in st_new:
                    tr.stats.sac = {}
                    tr.stats.sac['o'] = 0.0e0 # 发震时刻O
                    tr.stats.sac['a'] = -12345.0 # P 波到时a,-12345.0表示无效
                    tr.stats.sac['t0'] = -12345.0 # S 波到时t0,-12345.0e0
                    tr.stats.sac['evla'] = float(evla)
                    tr.stats.sac['evlo'] = float(evlo)
                    tr.stats.sac['evdp'] = float(evdp)
                    tr.stats.sac['mag'] = float(mag)
                    tr.stats.sac['stla'] = stla
                    tr.stats.sac['stlo'] = stlo
                    tr.stats.sac['stel'] = stel
                    tr.stats.sac['user0'] = 0.0 # 定义BCSEIS中的user0
                    tr.stats.sac['user1'] = 0.0 # 定义BCSEIS中的user1
                    tr.stats.sac['dist'] = distance
                    tr.stats.sac['az'] = dist.az # 事件到台站的方位角
                    tr.stats.sac['baz'] = dist.baz # 台站到事件的方位角
                    tr.stats.sac['gcarc'] = dist.delta # 台站到事件的大圆弧长度
                    for pick_id in picks:
                        pick = getPick(picks=event.picks,pick_id=pick_id)
                        arrival = getArrivalForPick(event.preferred_origin().arrivals,pick)
                        time = pick.time
                        phase_hint = pick.phase_hint
                        if phase_hint == 'P':
                            tr.stats.sac['a'] = float(time-origin_time)
                            tr.stats.sac['user0'] = arrival.time_weight
                            tr.stats.sac['ka'] = phase_hint
                        if phase_hint == 'S':
                            tr.stats.sac['t0'] = float(time-origin_time)
                            tr.stats.sac['user1'] = arrival.time_weight
                            tr.stats.sac['kt0'] = phase_hint
                for tr in st_new:
                    sacname = '.'.join([tr.stats.station,tr.stats.channel[-1],'SAC'])
                    tr.detrend('linear')
                    tr.detrend('demean')
                    tr.interpolate(100)
                    tr.write(datadir+os.sep+sacname,format='SAC')
    # write out the station
    with open(basedir+os.sep+'Stations/station.dat','w') as f:
        f.writelines(staline)
    with open(basedir+os.sep+'Events/IdDirFiles/iddir.dat','w') as f:
        f.writelines(iddir)
    with open(basedir+os.sep+'Events/EventInfo/events.catalog','w') as f:
        f.writelines(events_catalog)
def calcCCEvInfo3(basedir=None):
    # 对地震目录记录的P和S进行分析统计
    # calcCCEvInfo3.pl功能python实现
    
    warnFile = basedir+os.sep+'Events'+os.sep+'CCEventInfo'+os.sep+'ccEvInfo.warn'
    warnFile_f = open(warnFile,'w')
    
    ccEvInfoFile = basedir+os.sep+'Events'+os.sep+'CCEventInfo'+os.sep+'ccEvInfo.dat'
    ccEvInfoFile_f = open(ccEvInfoFile,'w')
    
    # 
    evInfoFile = basedir+os.sep+'Events'+os.sep+'EventInfo'+os.sep+'events.catalog'
    idDirFile = basedir+os.sep+'Events'+os.sep+'IdDirFiles'+os.sep+'iddir.dat'
    
    evInfoFile_pd = pd.read_csv(evInfoFile,sep='\s+',
                                names=['ev_id','yr','mn','dy','hr','mi','sc','evla','evlo','evdp','mag'],
                                dtype={'ev_id': str})
    #20190001 2019 09 07 00 00 13.472745 29.55192 104.60535  2.7017  2.55
    
    staFile = basedir+os.sep+'Stations'+os.sep+'station.dat'
    staFile_pd = pd.read_csv(staFile,sep='\s+',names=['sta','stla','stlo','stel'])
    #DZT   29.6365 104.9739 0.3470
    
    with open(idDirFile,'r') as f:
        for line in f:
            evId,data_dir = line.split()
            dataDir = basedir+os.sep+'Events'+os.sep+'Data'+os.sep+data_dir
            evLine = evInfoFile_pd[evInfoFile_pd['ev_id']==evId]
            if len(evLine) == 0:
                warnFile_f.write(">>> %s: No event info available.\n" %(evId))
                print("-- deleted (no event info available)")
                continue
            ## get the catalog event location info
            ## get the station list
            pStaList = getStaListInDir2(dirName=dataDir,phaCode=0)
            sStaList = getStaListInDir2(dirName=dataDir,phaCode=1)
            
            goodList1 = []
            if len(pStaList) == 0:
                warnFile_f.write(">>> %s: No station available with P pick\n" %(evId))
                print("-- deleted (no station available with P pick)")
            else:
                for sta in pStaList:
                    if sta in list(staFile_pd['sta']):
                        goodList1.append(sta)
            goodNum1 = len(goodList1)
            
            goodList2 = []
            if len(sStaList) == 0:
                warnFile_f.write(">>> %s: No station available with S pick\n" %(evId))
                print("-- deleted (no station available with P pick)")
            else:
                for sta in sStaList:
                    if sta in list(staFile_pd['sta']):
                        goodList2.append(sta)
            goodNum2 = len(goodList2)
            
            # write out the data
            ccEvInfoFile_f.write("# %s %8.4f %9.4f %7.3f %s %d %d\n" %(evId,evLine.evla.values[0],evLine.evlo.values[0],evLine.evdp.values[0],data_dir,goodNum1,goodNum2))
            if goodNum1 > 0:
                ccEvInfoFile_f.write('\n'.join(goodList1))
                ccEvInfoFile_f.write('\n')
            if goodNum2 > 0:
                ccEvInfoFile_f.write('\n'.join(goodList2))
                ccEvInfoFile_f.write('\n')
    warnFile_f.close()
    ccEvInfoFile_f.close()
def runBCSEIS(basedir=None):
    import subprocess
    import os
    iddir = basedir+os.sep+'Events/IdDirFiles/iddir.dat'
    bcseisid = basedir+os.sep+'Events'+os.sep+'WaveformCC'+os.sep+'bcseis.id'
    cur = os.getcwd()
    #shutil.copy('src'+os.sep+'bcseis',basedir+os.sep+'Events'+os.sep+'WaveformCC')
    #shutil.copy('src'+os.sep+'setsachdr',basedir+os.sep+'Events'+os.sep+'WaveformCC')
    #shutil.copy('src'+os.sep+'getsachdr',basedir+os.sep+'Events'+os.sep+'WaveformCC')
    with open(bcseisid,'w') as f:
        with open(iddir,'r') as f1:
            for line in f1:
                evid,etdir = line.split()
                f.write("%s\n" % evid)
    #baseDir='/mnt/e/Python/REAL-master/REAL-master/demo_real/BCSEISWY'
    baseDir = os.getcwd() + os.sep + basedir
    
    distCutoff=5
    preNum1=30
    posNum1=97
    preNum2=50
    posNum2=141
    segLen1=64
    segLen2=128
    lf=1
    hf=15
    velCode='NAN'
    startNum=1
    cmd = ' '.join(['bcseis','0',baseDir,str(distCutoff),str(preNum1),str(posNum1),
                    str(preNum2),str(posNum2),str(segLen1),str(segLen2),
                    str(lf),str(hf),str(velCode),str(startNum)])
    print(cmd)
    os.chdir(basedir+os.sep+'Events'+os.sep+'WaveformCC')
    print(os.getcwd())
    p = subprocess.Popen(cmd,shell=True)
    p.communicate()
    os.chdir(cur)
    #bcseis 0 $baseDir $distCutoff $preNum1 $posNum1 $preNum2 $posNum2 $segLen1 $segLen2 $lf $hf $velCode $startNum
def getDtFromCC(basedir=None,option=None,lCCLim=None,cCCLim=None,uCCLim=None,diffLim=None,dtFile=None):
    #Script:	getDtFromCC.pl
    #Author:	Wen-xuan(Wayne) Du
    #Date:		February 5, 2003
    #Last Modified:	September 16, 2003
    """
    print "Usage:\t\t$script <option> <ccDirList> <lCCLim> <cCCLim> <uCCLim> <diffLim> <evIdList> <dtFile>\n";
	print "<option>:\t0 - no limit control over CC-BS lag difference;\n";
	print "         \t1 - CC-BS lag difference 1 || 2 <= diffLim;\n";
	print "         \t2 - CC-BS lag difference 1 <= diffLim;\n";
	print "         \t3 - CC-BS lag difference 1 && 2 <= diffLim;\n";
	print "<ccDirList>:\tlist of directory names where CC results are kept;\n";
	print "<lCCLim>:\tlower CC limit\n";
	print "<cCCLim>:\tcenter CC limit;\n";
	print "<uCCLim>:\tupper CC limit;\n";
	print "<diffLim>:\tlimit in CC-BS lag difference;\n";
	print "<evIdList>:\tevent id list;\n";
	print "<dtFile>:\toutput dt.cc file;\n";
    """
    if ((option != 0) & (option != 1) & (option != 2) & (option != 3)):
        print(">>> Error: incorrect 'option' in command line: should be 0, 1, 2 or 3! option=%d\n" % option)
        return None
    # make the bcseis.id
    iddir = basedir+os.sep+'Events/IdDirFiles/iddir.dat'
    bcseisid = basedir+os.sep+'Events'+os.sep+'WaveformCC'+os.sep+'bcseis.id'
    id_list = []
    with open(bcseisid,'w') as f:
        with open(iddir,'r') as f1:
            for line in f1:
                evid,etdir = line.split()
                if evid not in id_list:
                    id_list.append(evid)
                f.write("%s\n" % evid)
    # write the resuts to file
    dtcc = basedir+os.sep+'Events'+os.sep+'WaveformCC'+os.sep+dtFile
    with open(dtcc,'w') as f_dtcc:
        ## deal with the event id list
        ## deal with CC files
        IdList = [line.split()[0] for line in open(bcseisid,'r').readlines()]
        for i,evId in enumerate(IdList):
            print(">>> processing event # %d: %s" %(i+1,evId))
            ccFile = basedir+os.sep+'Events'+os.sep+'WaveformCC'+os.sep+evId+'.CC'
            if not os.path.exists(ccFile):
                continue
            # 读取数据文件夹，进行BS验证和统计
            maxCC = 0.0
            evId2 = ''
            num = 0
            ccInfo = []
            with open(ccFile,'r') as fcc:
                for line in fcc:
                    if (line[0] == '#'):
                        if (evId2 != ''):
                            ccInfo.append([evId2,0,[maxCC,num]])
                        evId2 = line.split()[2]
                        maxCC = 0.0
                        num = 0
                    else:
                        num += 1
                        sta,ccdt,corv,phaName,ccLag1,bsLag1,bsLag2,sublag,sigma = line.split()
                        if float(corv) > maxCC:
                            maxCC = float(corv)
                        ccInfo.append([evId2,num,[sta,float(ccdt),float(corv),phaName,int(ccLag1),int(ccLag1),int(bsLag2),float(sublag),float(sigma)]])
            ccInfo.append([evId2,0,[maxCC,num]])
            ccInfo = pd.DataFrame(data=ccInfo,columns=['evId2','num','data'])
            # 
            for evId2 in sorted(set(list(ccInfo['evId2']))):
                if evId2 not in id_list:
                    continue
                bsFlag = 0 # cCCLim <= maxCC < uCCLim; 1 if maxCC >= uCCLim
                maxCC = ccInfo[(ccInfo['evId2'] == evId2) & (ccInfo['num'] == 0)]['data'].values[0][0]
                num = ccInfo[(ccInfo['evId2'] == evId2) & (ccInfo['num'] == 0)]['data'].values[0][1]
                
                #
                if maxCC < cCCLim:continue
                if maxCC >= uCCLim:bsFlag = 1
                goodCCInfo = []
                for j in range(1,num+1):
                    data = ccInfo[(ccInfo['evId2'] == evId2) & (ccInfo['num'] == j)]['data'].values[0]
                    sta = data[0]
                    ccdt = data[1]
                    corv = data[2]
                    phaName = data[3]
                    ccLag1 = data[4]
                    bsLag1 = data[5]
                    bsLag2 = data[6]
                    lagDiff1 = abs(ccLag1 - bsLag1) #  would be huge if $bsLag1 == -999 (error)
                    lagDiff2 = abs(ccLag1 - bsLag2) #  would be huge if $bsLag2 == -999 (error)
                    outLine = "%-5s %8.4f %8.4f %3s\n" % (sta,ccdt,corv,phaName)
                    
                    if option == 0:
                        if corv >= cCCLim: goodCCInfo.append(outLine)
                    else:
                        if bsFlag == 1:
                            if corv < lCCLim:continue
                        else:
                            if corv < cCCLim:continue
                        if option == 1:
                            if ((lagDiff1 <= diffLim) | (lagDiff2 <= diffLim)): goodCCInfo.append(outLine)
                        elif option==2:
                            if (lagDiff1 <= diffLim):goodCCInfo.append(outLine)
                        elif (option == 3):
                            if ((lagDiff1 <= diffLim) & (lagDiff2 <= diffLim)): goodCCInfo.append(outLine)
                if (len(goodCCInfo) > 0):
                    f_dtcc.write("# %s %s 0.0\n" %(evId,evId2))
                    f_dtcc.writelines(goodCCInfo)
    return None
if __name__ == '__main__':
    # 创建文件系统，指定工作目录所在的位置，后续数据处理和计算都早该文件夹下
    # 文件夹具体的组件模式参看使用手册
    basedir = 'Sanxia'
    print('Step1: making BCSEIS working...')
    makeBCSEIS(bcseisdir=basedir)
    
    # 数据文件与格式转换
    # 将ASDF格式的数据转换成BCSEIS相关的数据文件，地震目录和走时等信息的转换，同时对波形进行处理
    asdf = '7yue.hdf5'
    
    print('Step2: building Events...')
    buildEvents(basedir=basedir,asdf=asdf)
    
    # 台站记录震相统计与滤波
    print('Step3: get Stations statics...')
    getStaStat(basedir=basedir)
    
    # 从波形文件中获取CC计算文件
    print('Step4: calcCCEvInfo...')
    calcCCEvInfo3(basedir=basedir)
    # 这步可以从hypoDD中直接获取
    
    # run BCSEIS
    print('Sterp 5: Run BCSEIS...')
    runBCSEIS(basedir=basedir)
    
    # 
    # 生成 dt.cc
    print('Step 6: get Dt from cc...')
    ccInfo = getDtFromCC(basedir=basedir,option=0,lCCLim=0.5,cCCLim=0.7,uCCLim=0.8,diffLim=100,dtFile='dt_cc.cc0')
    #ccInfo = getDtFromCC(basedir=basedir,option=1,lCCLim=0.5,cCCLim=0.7,uCCLim=0.8,diffLim=100,dtFile='dt_cc.cc1')
    #ccInfo = getDtFromCC(basedir=basedir,option=2,lCCLim=0.5,cCCLim=0.7,uCCLim=0.8,diffLim=100,dtFile='dt_cc.cc2')
    ccInfo = getDtFromCC(basedir=basedir,option=3,lCCLim=0.5,cCCLim=0.7,uCCLim=0.8,diffLim=100,dtFile='dt_bs.cc3')   