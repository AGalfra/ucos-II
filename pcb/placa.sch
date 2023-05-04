EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R1
U 1 1 610E83CB
P 4100 2250
F 0 "R1" H 4170 2296 50  0000 L CNN
F 1 "4K7" H 4170 2205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4030 2250 50  0001 C CNN
F 3 "~" H 4100 2250 50  0001 C CNN
	1    4100 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 610E99EE
P 2550 4750
F 0 "#PWR02" H 2550 4500 50  0001 C CNN
F 1 "GND" H 2555 4577 50  0000 C CNN
F 2 "" H 2550 4750 50  0001 C CNN
F 3 "" H 2550 4750 50  0001 C CNN
	1    2550 4750
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 610EAC71
P 2500 2000
F 0 "#PWR01" H 2500 1850 50  0001 C CNN
F 1 "+3.3V" H 2515 2173 50  0000 C CNN
F 2 "" H 2500 2000 50  0001 C CNN
F 3 "" H 2500 2000 50  0001 C CNN
	1    2500 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2850 5900 2850
Wire Wire Line
	6100 2950 5900 2950
Wire Wire Line
	6100 3050 5900 3050
Wire Wire Line
	6100 3150 5900 3150
$Comp
L power:GND #PWR013
U 1 1 610EE75D
P 6100 2850
F 0 "#PWR013" H 6100 2600 50  0001 C CNN
F 1 "GND" V 6105 2722 50  0000 R CNN
F 2 "" H 6100 2850 50  0001 C CNN
F 3 "" H 6100 2850 50  0001 C CNN
	1    6100 2850
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR014
U 1 1 610EE763
P 6100 2950
F 0 "#PWR014" H 6100 2800 50  0001 C CNN
F 1 "+3.3V" V 6115 3078 50  0000 L CNN
F 2 "" H 6100 2950 50  0001 C CNN
F 3 "" H 6100 2950 50  0001 C CNN
	1    6100 2950
	0    1    1    0   
$EndComp
Text Label 5950 3050 0    50   ~ 0
SCL
Text Label 5950 3150 0    50   ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 610EF615
P 3650 2500
F 0 "J1" H 3568 2175 50  0000 C CNN
F 1 "Conn_01x03" H 3568 2266 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 3650 2500 50  0001 C CNN
F 3 "~" H 3650 2500 50  0001 C CNN
	1    3650 2500
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR010
U 1 1 610F1648
P 4100 2600
F 0 "#PWR010" H 4100 2350 50  0001 C CNN
F 1 "GND" V 4105 2472 50  0000 R CNN
F 2 "" H 4100 2600 50  0001 C CNN
F 3 "" H 4100 2600 50  0001 C CNN
	1    4100 2600
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR03
U 1 1 610F6C1F
P 3950 1950
F 0 "#PWR03" H 3950 1800 50  0001 C CNN
F 1 "+3.3V" H 3965 2123 50  0000 C CNN
F 2 "" H 3950 1950 50  0001 C CNN
F 3 "" H 3950 1950 50  0001 C CNN
	1    3950 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2400 3850 2400
Wire Wire Line
	3950 1950 3950 2050
Wire Wire Line
	4100 2100 4100 2050
Wire Wire Line
	4100 2050 3950 2050
Connection ~ 3950 2050
Wire Wire Line
	3950 2050 3950 2400
Wire Wire Line
	3850 2500 4100 2500
Wire Wire Line
	4100 2500 4100 2400
Wire Wire Line
	4350 2500 4100 2500
Connection ~ 4100 2500
Wire Wire Line
	4100 2600 3850 2600
Text Label 4250 2500 0    50   ~ 0
DQ
$Comp
L Modulos~Varios:Blue_Pill U1
U 1 1 610DF990
P 2450 3200
F 0 "U1" H 2450 1761 50  0000 C CNN
F 1 "Blue_Pill" H 2450 1670 50  0001 C CNN
F 2 "Modulos Varios:Blue_Pill" H 1550 4000 50  0001 C CNN
F 3 "" H 1750 4200 50  0001 C CNN
	1    2450 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 4550 2550 4650
Wire Wire Line
	2350 4550 2350 4650
Wire Wire Line
	2350 4650 2450 4650
Connection ~ 2550 4650
Wire Wire Line
	2550 4650 2550 4750
Wire Wire Line
	2450 4550 2450 4650
Connection ~ 2450 4650
Wire Wire Line
	2450 4650 2550 4650
Wire Wire Line
	2500 2000 2500 2100
Wire Wire Line
	2400 2200 2400 2100
Wire Wire Line
	2400 2100 2500 2100
Connection ~ 2500 2100
Wire Wire Line
	2500 2100 2500 2200
NoConn ~ 2150 2200
NoConn ~ 2750 2200
Wire Wire Line
	1600 3300 1950 3300
Wire Wire Line
	1600 3200 1950 3200
Wire Wire Line
	1600 2900 1950 2900
Wire Wire Line
	1600 2700 1950 2700
Wire Wire Line
	1600 2600 1950 2600
Wire Wire Line
	3200 4000 2950 4000
Text Label 3050 4000 0    50   ~ 0
DQ
Text Label 1650 3200 0    50   ~ 0
SCL
Text Label 1650 3300 0    50   ~ 0
SDA
Text Label 1650 2900 0    50   ~ 0
EBUT
Text Label 1650 2600 0    50   ~ 0
ENCA
Text Label 1650 2700 0    50   ~ 0
ENCB
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 610EA77E
P 4750 4200
F 0 "J5" H 4668 4517 50  0000 C CNN
F 1 "UART" H 4668 4426 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4750 4200 50  0001 C CNN
F 3 "~" H 4750 4200 50  0001 C CNN
	1    4750 4200
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 610ED2AD
P 5150 4400
F 0 "#PWR012" H 5150 4150 50  0001 C CNN
F 1 "GND" V 5155 4272 50  0000 R CNN
F 2 "" H 5150 4400 50  0001 C CNN
F 3 "" H 5150 4400 50  0001 C CNN
	1    5150 4400
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR011
U 1 1 610ED2B3
P 5150 4100
F 0 "#PWR011" H 5150 3950 50  0001 C CNN
F 1 "+3.3V" V 5165 4228 50  0000 L CNN
F 2 "" H 5150 4100 50  0001 C CNN
F 3 "" H 5150 4100 50  0001 C CNN
	1    5150 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 4100 4950 4100
Wire Wire Line
	5200 4200 4950 4200
Wire Wire Line
	5200 4300 4950 4300
Wire Wire Line
	5150 4400 4950 4400
Wire Wire Line
	3200 3500 2950 3500
Wire Wire Line
	3200 3600 2950 3600
Text Label 3050 3500 0    50   ~ 0
TX
Text Label 3050 3600 0    50   ~ 0
RX
Text Label 5050 4200 0    50   ~ 0
RX
Text Label 5050 4300 0    50   ~ 0
TX
$Comp
L Connector_Generic:Conn_01x03 J2
U 1 1 611018B2
P 3650 3100
F 0 "J2" H 3568 2775 50  0000 C CNN
F 1 "Conn_01x03" H 3568 2866 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 3650 3100 50  0001 C CNN
F 3 "~" H 3650 3100 50  0001 C CNN
	1    3650 3100
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR05
U 1 1 611065BC
P 4000 3200
F 0 "#PWR05" H 4000 2950 50  0001 C CNN
F 1 "GND" V 4005 3072 50  0000 R CNN
F 2 "" H 4000 3200 50  0001 C CNN
F 3 "" H 4000 3200 50  0001 C CNN
	1    4000 3200
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR04
U 1 1 611065C2
P 4000 3000
F 0 "#PWR04" H 4000 2850 50  0001 C CNN
F 1 "+3.3V" V 4015 3128 50  0000 L CNN
F 2 "" H 4000 3000 50  0001 C CNN
F 3 "" H 4000 3000 50  0001 C CNN
	1    4000 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	4000 3000 3850 3000
Wire Wire Line
	3850 3200 4000 3200
Wire Wire Line
	4000 3100 3850 3100
Text Label 3900 3100 0    50   ~ 0
DQ
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 6110E9DE
P 3650 3700
F 0 "J3" H 3568 3375 50  0000 C CNN
F 1 "Conn_01x03" H 3568 3466 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 3650 3700 50  0001 C CNN
F 3 "~" H 3650 3700 50  0001 C CNN
	1    3650 3700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR07
U 1 1 6110E9E4
P 4000 3800
F 0 "#PWR07" H 4000 3550 50  0001 C CNN
F 1 "GND" V 4005 3672 50  0000 R CNN
F 2 "" H 4000 3800 50  0001 C CNN
F 3 "" H 4000 3800 50  0001 C CNN
	1    4000 3800
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR06
U 1 1 6110E9EA
P 4000 3600
F 0 "#PWR06" H 4000 3450 50  0001 C CNN
F 1 "+3.3V" V 4015 3728 50  0000 L CNN
F 2 "" H 4000 3600 50  0001 C CNN
F 3 "" H 4000 3600 50  0001 C CNN
	1    4000 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	4000 3600 3850 3600
Wire Wire Line
	3850 3800 4000 3800
Wire Wire Line
	4000 3700 3850 3700
Text Label 3900 3700 0    50   ~ 0
DQ
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 6110FC99
P 3650 4300
F 0 "J4" H 3568 3975 50  0000 C CNN
F 1 "Conn_01x03" H 3568 4066 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Horizontal" H 3650 4300 50  0001 C CNN
F 3 "~" H 3650 4300 50  0001 C CNN
	1    3650 4300
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 6110FC9F
P 4000 4400
F 0 "#PWR09" H 4000 4150 50  0001 C CNN
F 1 "GND" V 4005 4272 50  0000 R CNN
F 2 "" H 4000 4400 50  0001 C CNN
F 3 "" H 4000 4400 50  0001 C CNN
	1    4000 4400
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 6110FCA5
P 4000 4200
F 0 "#PWR08" H 4000 4050 50  0001 C CNN
F 1 "+3.3V" V 4015 4328 50  0000 L CNN
F 2 "" H 4000 4200 50  0001 C CNN
F 3 "" H 4000 4200 50  0001 C CNN
	1    4000 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	4000 4200 3850 4200
Wire Wire Line
	3850 4400 4000 4400
Wire Wire Line
	4000 4300 3850 4300
Text Label 3900 4300 0    50   ~ 0
DQ
Text Label 6050 4400 0    50   ~ 0
ENCB
Text Label 6050 4300 0    50   ~ 0
ENCA
Text Label 6050 4200 0    50   ~ 0
EBUT
Wire Wire Line
	6000 4100 6150 4100
Wire Wire Line
	6000 4000 6150 4000
Wire Wire Line
	6250 4400 6000 4400
Wire Wire Line
	6250 4300 6000 4300
Wire Wire Line
	6250 4200 6000 4200
$Comp
L power:+3.3V #PWR016
U 1 1 610EA191
P 6150 4100
F 0 "#PWR016" H 6150 3950 50  0001 C CNN
F 1 "+3.3V" V 6165 4228 50  0000 L CNN
F 2 "" H 6150 4100 50  0001 C CNN
F 3 "" H 6150 4100 50  0001 C CNN
	1    6150 4100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR015
U 1 1 610E8FB6
P 6150 4000
F 0 "#PWR015" H 6150 3750 50  0001 C CNN
F 1 "GND" V 6155 3872 50  0000 R CNN
F 2 "" H 6150 4000 50  0001 C CNN
F 3 "" H 6150 4000 50  0001 C CNN
	1    6150 4000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J6
U 1 1 610E65E4
P 5800 4200
F 0 "J6" H 5718 4617 50  0000 C CNN
F 1 "Encoder" H 5718 4526 50  0000 C CNN
F 2 "Modulos Varios:Encoder_Rotativo" H 5800 4200 50  0001 C CNN
F 3 "~" H 5800 4200 50  0001 C CNN
	1    5800 4200
	-1   0    0    -1  
$EndComp
$Comp
L Modulos~Varios:SSD1306 U2
U 1 1 610E5C14
P 5600 3000
F 0 "U2" H 5382 3417 50  0000 C CNN
F 1 "SSD1306" H 5382 3326 50  0000 C CNN
F 2 "Modulos Varios:SSD1306" H 5650 3300 50  0001 C CNN
F 3 "" H 5650 3300 50  0001 C CNN
	1    5600 3000
	1    0    0    -1  
$EndComp
NoConn ~ 2950 2600
NoConn ~ 2950 2700
NoConn ~ 2950 2800
NoConn ~ 2950 2900
NoConn ~ 2950 3000
NoConn ~ 2950 3100
NoConn ~ 2950 3200
NoConn ~ 2950 3300
NoConn ~ 2950 3400
NoConn ~ 2950 3700
NoConn ~ 2950 3800
NoConn ~ 2950 4150
NoConn ~ 2950 4250
NoConn ~ 2950 4350
NoConn ~ 1950 4300
NoConn ~ 1950 4100
NoConn ~ 1950 4000
NoConn ~ 1950 3900
NoConn ~ 1950 3800
NoConn ~ 1950 3700
NoConn ~ 1950 3600
NoConn ~ 1950 3500
NoConn ~ 1950 3400
NoConn ~ 1950 3100
NoConn ~ 1950 3000
Text Notes 3650 4600 0    50   ~ 0
BUS ONEWIRE
$EndSCHEMATC
