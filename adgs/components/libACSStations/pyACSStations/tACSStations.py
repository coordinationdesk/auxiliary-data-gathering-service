from pySgiIncludes import pySgi_Spot123Info, pySgi_Spot4Info, pySgi_Data_Transcription_Statistic, pySgi_Transcription_File_Description_Header

a = pySgi_Data_Transcription_Statistic()

a = pySgi_Transcription_File_Description_Header()
a.read("/raid0/DI/workdirs/89/ER02_SAR_IM__0P_19970923T212658_19970923T213315_KSE_7339_D7F5.WILM/DTUserHeader.dat")
a.get_orbit_number()
a.set_orbit_number(22)
a.get_orbit_number()
a.write("/raid0/DI/workdirs/89/ER02_SAR_IM__0P_19970923T212658_19970923T213315_KSE_7339_D7F5.WILM/DTUserHeader.dat_ilfigliodi")
a.get_orbit_number()

s  = pySgi_Spot123Info()
## s.set_card05(0,1,2,3,4.,5.,6,7,8,9)
## s.set_card06(0,1,2,10,4,5,6,7,8.,9.,10)
## s.set_channel_segment(1,0,0,1,1,'S','S',0,1,"123", 90,1,0,1)
## s.set_card07(1,2,3,4,5.,6.,7,8,9,10)
## s.set_osculating_element(0,"2345",6.0,"789",10)
## s.set_osculating_element(1,"2345",6.0,"789",10)
## s.set_osculating_element(2,"2345",6.0,"789",10)
## s.set_ephemeris_point(0,1.,2.,3.,4.,5.,6.,7)
s.write("/tmp/ciccio.out")

s4 = pySgi_Spot4Info()
## s4.set_card02(33)
## s4.set_card05(0,"S4 ", 1,2,3.0, 4., 5,6,7,8)
## s4.set_card06(0,"S4 ", 1,20,3,4,6.)
## s4.set_channel_segment("C1",10,20,20,1.,2.,3.,4.,5.,6.,7.)
## s4.set_reserv_slot(0, 1, "S4", 100, 'D', 10., 20., 30., 40.)
## s4.set_osculating_element(0,"2345",6.0,"789",10)
## s4.set_ephemeris_point(0,1.,2.,3.,4.,5.,6.,5)
s.write("/tmp/ciccio4.out")


#s4 = lkajlakfh
#s4.set_all_structure()

