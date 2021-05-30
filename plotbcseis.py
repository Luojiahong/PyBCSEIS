# plot the CC
import pygmt
import os
from obspy import read

basedir = 'Sanxia'
# 定义需要绘图的文件
ccfile = basedir+os.sep+'Events'+os.sep+'WaveformCC'+os.sep+'draw.cc3'
dirf = basedir+os.sep+'Events'+os.sep+'IdDirFiles'+os.sep+'iddir.dat'
pygmt.config(FONT='7p,black',FONT_ANNOT='7p,black',FONT_LABEL='7p,black')
with open(ccfile,'r') as f:
    for line in f:
        flag = line[0]
        if flag == '#' :
            flag,id1,id2,tt = line.split()
        else:
            sta,dt,cc,pha = line.split()
            with open(dirf,'r') as f1:
                for line1 in f1:
                    flag1,dirs = line1.split()
                    if flag1 == id1:
                        dir1 = dirs
                    if flag1 == id2:
                        dir2 = dirs
            sac1 = basedir+os.sep+'Events'+os.sep+'Data'+os.sep+dir1+os.sep+'filt.'+sta+'.Z.SAC'
            sac2 = basedir+os.sep+'Events'+os.sep+'Data'+os.sep+dir2+os.sep+'filt.'+sta+'.Z.SAC'
            #print(sac1,sac2)
            title3 = "CC="+str(cc)+",DT="+str(dt)
            if pha == "P":
                name = "figures"+os.sep+sta+'_P_'+str(id1)+'_'+str(id2)
                tr1 = read(sac1)[0]
                tp = tr1.stats.sac.a
                tr2 = read(sac2)[0]
                tp1 = tr2.stats.sac.a
            if pha == 'S':
                name = "figures"+os.sep+sta+'_S_'+str(id1)+'_'+str(id2)
                tr1 = read(sac1)[0]
                tp = tr1.stats.sac.t0
                tr2 = read(sac2)[0]
                tp1 = tr2.stats.sac.t0
            # 设置时窗长度，注意与实际计算长度对应
            t1 = tp - 0.15
            t2 = tp + 0.485
            t11 = tp1 - 0.15
            t21 = tp1 + 0.485
            dtt = tp - tp1
            title4 = "CAT_DT="+str(dtt)
            # 绘制地震波形
            print(name)
            fig = pygmt.Figure()
            fig.basemap(region=[t1,t2,-5000,5000],projection='X2.5i/0.8i',frame=['xa0.1','ya1000+lCounts','N'])
            with pygmt.clib.Session() as ses:
                cmd = "%s -C%f/%f -Tt-3 -W0.5p,black -M1 -Fr" %(sac1,t1,t2)
                ses.call_module('sac',cmd)
            fig.plot(x=[tp,tp],y=[0,5000],pen='0.5p,black')
            fig.text(x=tp,y=4000,text=pha+'='+str(tp),font='7p',justify='LM',offset='j0.1c/0.0c')
            fig.text(x=t1,y=3500,text=id1,font='7p',justify='LM')
            with pygmt.config(MAP_FRAME_PEN='thinner,red',MAP_FRAME_WIDTH='1p',MAP_TICK_PEN='red',FONT='7p,red'):
                fig.basemap(region=[t11,t21,-5000,5000],projection='X2.5i/0.8i',frame=['xa0.1','ya1000+lCounts','S'])
                with pygmt.clib.Session() as ses:
                    cmd = "%s -C%f/%f -Tt-3 -W0.5p,red -M1 -Fr" %(sac2,t11,t21)
                    ses.call_module('sac',cmd)
                fig.plot(x=[tp1,tp1],y=[-5000,0],pen='0.5p,red')
                fig.text(x=tp1,y=-4000,text=pha+'='+str(tp1),font='7p,red',justify='LM',offset='j0.1c/0.0c')
                fig.text(x=t11,y=2000,text=id2,font='7p,red',justify='LM')
            fig.text(x=t11,y=-3000,text=sta,font='10p,black',justify='LM')
            # 绘制第二个波形
            fig.shift_origin(yshift='-1.0i')
            fig.basemap(region=[t1,t2,-5000,5000],projection='X2.5i/0.8i',frame=['xa0.1+lTime(s)','ya1000+lCounts','S'])
            with pygmt.clib.Session() as ses:
                cmd = "%s -C%f/%f -Tt-3 -W0.5p,black -M1 -Fr" %(sac1,t1,t2)
                ses.call_module('sac',cmd)
                cmd = "%s -C%f/%f -Tt-3+s%f -W0.5p,red -M1 -Fr" %(sac2,t1,t2,float(dt))
                ses.call_module('sac',cmd)
            fig.text(x=t1,y=-2000,text=title3,font='7p',justify='LM')
            fig.text(x=t1,y=-4000,text=title4,font='7p',justify='LM')
            #fig.show()
            fig.savefig(name+'.png')