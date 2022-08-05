#!/usr/bin/env python

from samplebase import SampleBase
from rgbmatrix  import graphics
import time
from datetime   import datetime
from pytz       import timezone
import pytz
from xml.dom import minidom
format = ("%I:%M")

localtime = time.asctime( time.localtime(time.time()) )
doc = minidom.parse("/home/pi/settings.xml")

zone1 = doc.getElementsByTagName("zone1")
for zone in zone1:
        tz_name1 = zone.getElementsByTagName("tz_name")[0]
        tz_label1 = zone.getElementsByTagName("tz_label")[0]
zone2 = doc.getElementsByTagName("zone2")
for zone in zone2:
        tz_name2 = zone.getElementsByTagName("tz_name")[0]
        tz_label2 = zone.getElementsByTagName("tz_label")[0]
zone3 = doc.getElementsByTagName("zone3")
for zone in zone3:
        tz_name3 = zone.getElementsByTagName("tz_name")[0]
        tz_label3 = zone.getElementsByTagName("tz_label")[0]
zone4 = doc.getElementsByTagName("zone4")
for zone in zone4:
        tz_name4 = zone.getElementsByTagName("tz_name")[0]
        tz_label4 = zone.getElementsByTagName("tz_label")[0]
zone5 = doc.getElementsByTagName("zone5")
for zone in zone5:
        tz_name5 = zone.getElementsByTagName("tz_name")[0]
        tz_label5 = zone.getElementsByTagName("tz_label")[0]
zone6 = doc.getElementsByTagName("zone6")
for zone in zone6:
        tz_name6 = zone.getElementsByTagName("tz_name")[0]
        tz_label6 = zone.getElementsByTagName("tz_label")[0]
         
class RunText(SampleBase):
    def run(self):
        offscreen_canvas = self.matrix.CreateFrameCanvas()
        font = graphics.Font()
        font2 = graphics.Font()
        font3 = graphics.Font()
        font4 = graphics.Font()
        font.LoadFont("../../../fonts/6x13B.bdf")
        font2.LoadFont("../../../fonts/4x6.bdf")
        font3.LoadFont("../../../fonts/4x6.bdf")
        font4.LoadFont("../../../fonts/5x7.bdf")
        textRed =   graphics.Color(255,  0,  0)
        textWhite = graphics.Color(255,255,255)
        textYellow =graphics.Color(255,255,  0)
        
        while True:
            offscreen_canvas.Clear()           
            now_utc = datetime.now(timezone('UTC'))
            GMT = (now_utc.strftime(format))
                              
            zone1 = now_utc.astimezone(timezone(tz_name1.firstChild.data))
            time_zone1 = (zone1.strftime(format))
            ampm1 = (zone1.strftime("%p"))
            
            graphics.DrawText(offscreen_canvas, font,  0,  9, textRed,   time_zone1)
            graphics.DrawText(offscreen_canvas, font3,  31,  9, textRed, ampm1)
            graphics.DrawText(offscreen_canvas, font2,  6, 15, textWhite, tz_label1.firstChild.data)        
            
            zone2 = now_utc.astimezone(timezone(tz_name2.firstChild.data))
            time_zone2 = (zone2.strftime(format))
            ampm2 = (zone2.strftime("%p"))
            graphics.DrawText(offscreen_canvas, font,  0, 25, textRed,   time_zone2)
            graphics.DrawText(offscreen_canvas, font3,  31,  25, textRed, ampm2)
            graphics.DrawText(offscreen_canvas, font2,  9, 31, textWhite, tz_label2.firstChild.data)                 
            
            zone3 = now_utc.astimezone(timezone(tz_name3.firstChild.data))
            time_zone3 = (zone3.strftime(format))
            ampm3 = (zone3.strftime("%p"))
            graphics.DrawText(offscreen_canvas, font, 43,  9, textRed,   time_zone3)
            graphics.DrawText(offscreen_canvas, font3,  74,  9, textRed, ampm3)
            graphics.DrawText(offscreen_canvas, font2, 49, 15, textWhite, tz_label3.firstChild.data)            
            
            zone4 = now_utc.astimezone(timezone(tz_name4.firstChild.data))
            time_zone4 = (zone4.strftime(format))
            ampm4 = (zone4.strftime("%p"))
            graphics.DrawText(offscreen_canvas, font, 43, 25, textRed,   time_zone4)
            graphics.DrawText(offscreen_canvas, font3,  74,  25, textRed, ampm4)
            graphics.DrawText(offscreen_canvas, font2, 52, 31, textWhite, tz_label4.firstChild.data)            
            
            zone5 = now_utc.astimezone(timezone(tz_name5.firstChild.data))
            time_zone5 = (zone5.strftime(format))
            ampm5 = (zone5.strftime("%p"))            
            graphics.DrawText(offscreen_canvas, font, 87,  9, textRed,   time_zone5)
            graphics.DrawText(offscreen_canvas, font3,  118,  9, textRed, ampm5)
            graphics.DrawText(offscreen_canvas, font2, 85, 15, textWhite, tz_label5.firstChild.data)            
            
            zone6 = now_utc.astimezone(timezone(tz_name6.firstChild.data))
            time_zone6 = (zone6.strftime(format))
            ampm6 = (zone6.strftime("%p"))                 
            graphics.DrawText(offscreen_canvas, font, 87, 25, textRed,   time_zone6)
            graphics.DrawText(offscreen_canvas, font3,  118,  25, textRed, ampm6)
            graphics.DrawText(offscreen_canvas, font2, 90, 31, textWhite, tz_label6.firstChild.data)            
            
            ############ +/- day ###############
            
            #If time zones or location names are changed, the code below might not display the
            #plus or minus day correctly
            zone1_am = (zone1.strftime("%d"))
            zone2_am = (zone2.strftime("%d"))
            zone3_am = (zone3.strftime("%d"))
            zone4_am = (zone4.strftime("%d"))
            zone5_am = (zone5.strftime("%d"))
            zone6_am = (zone6.strftime("%d"))
            if zone1_am > zone6_am:
                graphics.DrawText(offscreen_canvas, font4, 40 , 15, textYellow, "-1")
            if zone1_am > zone6_am:
                graphics.DrawText(offscreen_canvas, font4, 76 , 15, textYellow, "-1")
            if zone1_am < zone2_am:
                graphics.DrawText(offscreen_canvas, font4, 0 , 31, textYellow, "+1")
            if zone1_am < zone4_am:
                graphics.DrawText(offscreen_canvas, font4, 42 , 31, textYellow, "+1")
            if zone1_am < zone6_am:
                graphics.DrawText(offscreen_canvas, font4, 80 , 31, textYellow, "+1")
            ##########  End +/- day  ###########
            
            time.sleep(1)
            offscreen_canvas = self.matrix.SwapOnVSync(offscreen_canvas)
        
if __name__ == "__main__":
    run_text = RunText()
    
    if (not run_text.process()):
        run_text.print_help()
        
        
        
