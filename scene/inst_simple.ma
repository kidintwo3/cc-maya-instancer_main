//Maya ASCII 2016R2 scene
//Name: inst_simple.ma
//Last modified: Sun, Jun 12, 2016 08:36:28 PM
//Codeset: 1252
requires maya "2016R2";
requires -nodeType "clonerMulti" "Instancer" "1.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2016";
fileInfo "version" "2016 Extension 2";
fileInfo "cutIdentifier" "201603022110-988944-2";
fileInfo "osv" "Microsoft Windows 8 Business Edition, 64-bit  (Build 9200)\n";
createNode transform -s -n "persp";
	rename -uid "2DD39C50-4B67-0248-282A-7ABE289A9638";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 29.00100151121628 27.278076224360049 -54.156636655633157 ;
	setAttr ".r" -type "double3" -21.338352729717759 131.79999999986228 0 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "AA33BEDD-4D16-1CB5-3117-C88F63D004C3";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 91.763150208242052;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" -0.015174388885498047 0.16376289307904202 0 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	rename -uid "53703712-4271-B10E-EA0E-81BA0891CE0F";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "89CDF01F-49AF-8C6F-C6FA-3D9E3AE5B87F";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	rename -uid "308F8EA6-4366-928F-8155-998A1BCE9DE0";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "8424AE48-405C-C2BC-6DB0-BE89F7348112";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	rename -uid "C82999FB-4C56-A626-3B6E-0D90C82CEB81";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "93E62C3F-499B-1658-7C5D-869156E45C0A";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pCube1";
	rename -uid "67009392-494E-D02E-CBD5-0783625E2EC1";
	setAttr ".t" -type "double3" 0.096974326541145572 1.5206171117666831 0 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
	rename -uid "B0597DCA-4A8E-E37C-6884-7D86E6904DEB";
	setAttr -k off ".v";
	setAttr -s 2 ".iog";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.5 0.875 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 104 ".uvst[0].uvsp[0:103]" -type "float2" 0.61884207 0.75 0.625
		 0.75615793 0.62499994 0.99384201 0.61884207 1 0.38115793 1 0.375 0.99384201 0.38115796
		 -6.5709369e-011 0.63115793 1.4784608e-009 0.375 0.25920153 0.61579847 0.25 0.625
		 0.49079844 0.38420153 0.5 0.875 0 0.875 0.017638743 0.625 0.73236126 0.625 0.50843477
		 0.375 0.75615793 0.375 0.73236126 0.375 0.50843477 0.375 0.25 0.375 0.49999997 0.625
		 0.25 0.625 0.49339646 0.625 0.5 0.12814879 0.24471404 0.13981549 0.25 0.13343474
		 0.25 0.87195849 0.24460678 0.86656523 0.25 0.38420153 0.25 0.38420153 0.25 0.625
		 0.25920153 0.625 0.25920153 0.61579847 0.5 0.61579847 0.5 0.375 0.49079847 0.375
		 0.49079847 0.61884207 -1.9712811e-010 0.61884207 0.017638724 0.86884201 1.3141874e-010
		 0.86884207 0.017638773 0.38115796 0.73236126 0.38115793 0.75 0.13115796 -6.2423966e-010
		 0.36884207 -4.9282078e-010 0.36884207 0.017638722 0.375 0.25920156 0.375 0.49171346
		 0.38328657 0.25 0.61579847 0.25 0.625 0.25828654 0.625 0.49079832 0.61671323 0.5
		 0.38420147 0.5 0.38420442 0.25 0.61579847 0.25 0.625 0.25919813 0.625 0.49079844
		 0.61579573 0.5 0.38420153 0.5 0.375 0.49079844 0.375 0.25920153 0.36861923 0.25 0.13115796
		 0.24156526 0.38115796 0.24156526 0.61839622 0.25 0.63959128 0.25 0.63115793 0.24156526
		 0.86040729 0.25 0.38160357 0.49999997 0.61884207 0.50843477 0.38115793 0.017638724
		 0.61884207 0.24156526 0.38115793 0.50843477 0.61884207 0.73236126 0.63115793 0.017638724
		 0.86884201 0.24156526 0.13115795 0.017638743 0.36884204 0.24156526 0.38160202 0.25
		 0.61671352 0.25 0.625 0.25660354 0.625 0.49171346 0.61839652 0.5 0.38328654 0.5 0.375
		 0.49339643 0.375 0.25660357 0.37500003 0.25828657 0.375 0.25920153 0.38420153 0.25
		 0.61579847 0.25 0.625 0.25920153 0.625 0.49079844 0.61579847 0.5 0.38420153 0.5 0.375
		 0.49079847 0.375 0.75615793 0.38115793 0.75 0.61884207 0.75 0.625 0.75615793 0.62499994
		 0.99384201 0.61884207 1 0.38115793 1 0.375 0.99384201;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".pt[72:79]" -type "float3"  0.025834994 -0.029050488 
		0.027173666 0.027173666 -0.029050488 0.025834993 -0.025834994 -0.029050488 0.027173666 
		-0.027173666 -0.029050488 0.025834993 -0.027173666 -0.029050488 -0.025834993 -0.025834994 
		-0.029050488 -0.027173666 0.025834994 -0.029050488 -0.027173666 0.027173666 -0.029050488 
		-0.025834993;
	setAttr -s 80 ".vt[0:79]"  -0.49180275 -0.49180281 0.46757475 -0.46757478 -0.49180281 0.49180275
		 -0.49180269 -0.42240465 0.46757475 -0.46757472 -0.42240465 0.49180272 0.49180275 -0.49180281 0.46757475
		 0.46757478 -0.49180281 0.49180275 0.46757478 -0.42240465 0.49180272 0.49180275 -0.42240465 0.46757472
		 -0.30490184 1.54357278 0.32912982 -0.32912982 1.54357278 0.30490181 -0.30490184 1.47057045 0.32912982
		 -0.32912982 1.47057045 0.30490181 0.32912982 1.54357278 0.30490181 0.30490178 1.54357278 0.32912982
		 0.32912982 1.47057045 0.30490181 0.30490178 1.47057045 0.32912982 0.30490178 1.54357278 -0.32912982
		 0.32912982 1.54357278 -0.30490181 0.30490178 1.47057045 -0.32912982 0.32912982 1.47057045 -0.30490181
		 -0.32912982 1.54357278 -0.30490181 -0.30490184 1.54357278 -0.32912982 -0.32912982 1.47057045 -0.30490181
		 -0.30490184 1.47057045 -0.32912982 0.49180275 -0.49180281 -0.46757475 0.46757478 -0.49180281 -0.49180275
		 0.49180275 -0.42240453 -0.46757475 0.46757478 -0.42240453 -0.49180275 -0.46757478 -0.49180281 -0.49180275
		 -0.49180275 -0.49180281 -0.46757475 -0.46757478 -0.42240453 -0.49180275 -0.49180275 -0.42240453 -0.46757475
		 -0.3412438 0.49180281 0.36547181 -0.36547178 0.49180281 0.3412438 -0.32912982 0.52814472 0.30490181
		 -0.30490184 0.52814472 0.32912982 -0.36547178 0.49180281 -0.3412438 -0.3412438 0.49180281 -0.36547181
		 -0.30490184 0.52814472 -0.32912982 -0.32912982 0.52814472 -0.30490181 0.36547178 0.49180281 0.3412438
		 0.3412438 0.49180281 0.36547181 0.30490178 0.52814472 0.32912982 0.32912982 0.52814472 0.30490181
		 0.3412438 0.49180281 -0.36547181 0.36547178 0.49180281 -0.3412438 0.32912982 0.52814472 -0.30490181
		 0.30490178 0.52814472 -0.32912982 -0.46757478 0.45861697 0.49180275 -0.49180275 0.45861697 0.46757475
		 -0.45861697 0.49180281 0.43438894 -0.434389 0.49180281 0.45861694 -0.49180275 0.45861697 -0.46757475
		 -0.46757478 0.45861697 -0.49180275 -0.434389 0.49180281 -0.45861694 -0.45861697 0.49180281 -0.43438894
		 0.49180275 0.45861697 0.46757475 0.46757478 0.45861697 0.49180275 0.43438894 0.49180281 0.45861694
		 0.45861691 0.49180281 0.43438897 0.46757478 0.45861697 -0.49180275 0.49180275 0.45861697 -0.46757475
		 0.45861691 0.49180281 -0.43438897 0.43438894 0.49180281 -0.45861694 -0.27674922 1.5764972 0.29874015
		 -0.29874015 1.5764972 0.27674919 0.27674919 1.5764972 0.29874015 0.29874015 1.5764972 0.27674919
		 0.29874015 1.5764972 -0.27674919 0.27674919 1.5764972 -0.29874015 -0.27674922 1.5764972 -0.29874015
		 -0.29874015 1.5764972 -0.27674919 -0.46757478 -0.49180281 -0.49180275 -0.49180275 -0.49180281 -0.46757475
		 0.46757478 -0.49180281 -0.49180275 0.49180275 -0.49180281 -0.46757475 0.49180275 -0.49180281 0.46757475
		 0.46757478 -0.49180281 0.49180275 -0.46757478 -0.49180281 0.49180275 -0.49180275 -0.49180281 0.46757475;
	setAttr -s 152 ".ed[0:151]"  1 5 0 1 0 0 4 5 0 8 13 0 9 20 0 8 9 0 12 17 0
		 13 12 0 17 16 0 21 16 0 21 20 0 24 4 0 25 24 0 28 25 0 29 0 0 28 29 0 1 3 0 3 2 1
		 2 0 0 3 48 0 48 49 0 49 2 0 4 7 0 7 6 1 6 5 0 7 56 0 56 57 0 57 6 0 9 11 0 11 10 1
		 10 8 0 11 34 0 34 35 0 35 10 0 13 15 0 15 14 1 14 12 0 15 42 0 42 43 0 43 14 0 17 19 0
		 19 18 1 18 16 0 19 46 0 46 47 0 47 18 0 21 23 0 23 22 1 22 20 0 23 38 0 38 39 0 39 22 0
		 25 27 0 27 26 1 26 24 0 27 60 0 60 61 0 61 26 0 29 31 0 31 30 1 30 28 0 31 52 0 52 53 0
		 53 30 0 32 33 0 33 50 1 50 51 0 51 32 1 32 35 1 34 33 1 36 37 0 37 54 1 54 55 0 55 36 1
		 36 39 1 38 37 1 40 41 0 41 58 1 58 59 0 59 40 1 40 43 1 42 41 1 44 45 0 45 62 1 62 63 0
		 63 44 1 44 47 1 46 45 1 48 51 0 50 49 0 52 55 0 54 53 0 56 59 1 58 57 0 60 63 0 62 61 1
		 10 15 1 14 19 1 18 23 1 22 11 1 6 3 1 26 7 1 30 27 1 2 31 1 34 39 0 36 33 0 32 41 0
		 42 35 0 40 45 0 46 43 0 44 37 0 38 47 0 50 55 0 52 49 0 48 57 0 58 51 0 56 61 0 62 59 0
		 54 63 0 60 53 0 8 64 0 9 65 0 64 65 0 13 66 0 64 66 0 12 67 0 66 67 0 17 68 0 67 68 0
		 16 69 0 68 69 0 21 70 0 70 69 0 20 71 0 70 71 0 65 71 0 28 72 0 29 73 0 72 73 0 25 74 0
		 72 74 0 24 75 0 74 75 0 4 76 0 75 76 0 5 77 0 76 77 0 1 78 0 78 77 0 0 79 0 78 79 0
		 73 79 0;
	setAttr -s 74 -ch 304 ".fc[0:73]" -type "polyFaces" 
		f 8 -123 124 126 128 130 -133 134 -136
		mu 0 8 88 89 90 91 92 93 94 95
		f 8 -139 140 142 144 146 -149 150 -152
		mu 0 8 96 97 98 99 100 101 102 103
		f 4 -2 16 17 18
		mu 0 4 44 6 71 45
		f 4 -18 19 20 21
		mu 0 4 45 71 64 78
		f 4 -3 22 23 24
		mu 0 4 37 7 75 38
		f 4 -24 25 26 27
		mu 0 4 38 75 67 72
		f 4 5 28 29 30
		mu 0 4 30 8 61 29
		f 4 -30 31 32 33
		mu 0 4 29 61 46 54
		f 4 -8 34 35 36
		mu 0 4 32 9 55 31
		f 4 -36 37 38 39
		mu 0 4 31 55 49 56
		f 4 -9 40 41 42
		mu 0 4 34 10 57 33
		f 4 -42 43 44 45
		mu 0 4 33 57 51 58
		f 4 -11 46 47 48
		mu 0 4 36 11 59 35
		f 4 -48 49 50 51
		mu 0 4 35 59 53 60
		f 4 -13 52 53 54
		mu 0 4 39 12 13 40
		f 4 -54 55 56 57
		mu 0 4 14 74 70 15
		f 4 15 58 59 60
		mu 0 4 42 16 17 41
		f 4 -60 61 62 63
		mu 0 4 41 17 18 73
		f 4 64 65 66 67
		mu 0 4 48 87 19 79
		f 4 -65 68 -33 69
		mu 0 4 87 48 54 46
		f 4 70 71 72 73
		mu 0 4 47 84 69 20
		f 4 -71 74 -51 75
		mu 0 4 84 47 60 53
		f 4 76 77 78 79
		mu 0 4 50 80 65 21
		f 4 -77 80 -39 81
		mu 0 4 80 50 56 49
		f 4 82 83 84 85
		mu 0 4 52 82 22 23
		f 4 -83 86 -45 87
		mu 0 4 82 52 58 51
		f 4 -21 88 -67 89
		mu 0 4 78 64 79 62
		f 4 -63 90 -73 91
		mu 0 4 24 63 25 26
		f 4 -27 92 -79 93
		mu 0 4 72 67 21 65
		f 4 -57 94 -85 95
		mu 0 4 76 27 28 68
		f 4 96 -35 -4 -31
		mu 0 4 29 55 9 30
		f 4 97 -41 -7 -37
		mu 0 4 31 57 10 32
		f 4 98 -47 9 -43
		mu 0 4 33 59 11 34
		f 4 99 -29 4 -49
		mu 0 4 35 61 8 36
		f 4 -17 0 -25 100
		mu 0 4 71 6 37 38
		f 4 -23 -12 -55 101
		mu 0 4 75 7 39 40
		f 4 102 -53 -14 -61
		mu 0 4 41 74 0 42
		f 4 -59 14 -19 103
		mu 0 4 77 43 44 45
		f 4 -70 104 -75 105
		mu 0 4 87 46 60 47
		f 4 -69 106 -82 107
		mu 0 4 54 48 80 49
		f 4 -81 108 -88 109
		mu 0 4 56 50 82 51
		f 4 -87 110 -76 111
		mu 0 4 58 52 84 53
		f 4 -108 -38 -97 -34
		mu 0 4 54 49 55 29
		f 4 -110 -44 -98 -40
		mu 0 4 56 51 57 31
		f 4 -112 -50 -99 -46
		mu 0 4 58 53 59 33
		f 4 -105 -32 -100 -52
		mu 0 4 60 46 61 35
		f 4 -90 112 -91 113
		mu 0 4 78 62 25 63
		f 4 -89 114 -94 115
		mu 0 4 79 64 72 65
		f 4 -93 116 -96 117
		mu 0 4 66 67 76 68
		f 4 -92 118 -95 119
		mu 0 4 73 69 83 70
		f 4 -101 -28 -115 -20
		mu 0 4 71 38 72 64
		f 4 -120 -56 -103 -64
		mu 0 4 73 70 74 41
		f 4 -102 -58 -117 -26
		mu 0 4 75 40 76 67
		f 4 -104 -22 -114 -62
		mu 0 4 77 45 78 63
		f 4 -116 -78 -107 -68
		mu 0 4 79 65 80 48
		f 4 -118 -84 -109 -80
		mu 0 4 81 22 82 50
		f 4 -119 -72 -111 -86
		mu 0 4 83 69 84 52
		f 4 -113 -66 -106 -74
		mu 0 4 85 86 87 47
		f 4 -6 120 122 -122
		mu 0 4 8 30 89 88
		f 4 3 123 -125 -121
		mu 0 4 30 9 90 89
		f 4 7 125 -127 -124
		mu 0 4 9 32 91 90
		f 4 6 127 -129 -126
		mu 0 4 32 10 92 91
		f 4 8 129 -131 -128
		mu 0 4 10 34 93 92
		f 4 -10 131 132 -130
		mu 0 4 34 11 94 93
		f 4 10 133 -135 -132
		mu 0 4 11 36 95 94
		f 4 -5 121 135 -134
		mu 0 4 36 8 88 95
		f 4 -16 136 138 -138
		mu 0 4 16 42 97 96
		f 4 13 139 -141 -137
		mu 0 4 42 0 98 97
		f 4 12 141 -143 -140
		mu 0 4 0 1 99 98
		f 4 11 143 -145 -142
		mu 0 4 1 2 100 99
		f 4 2 145 -147 -144
		mu 0 4 2 3 101 100
		f 4 -1 147 148 -146
		mu 0 4 3 4 102 101
		f 4 1 149 -151 -148
		mu 0 4 4 5 103 102
		f 4 -15 137 151 -150
		mu 0 4 5 16 96 103;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".bw" 3;
	setAttr ".dr" 1;
createNode transform -n "pPyramid1";
	rename -uid "555B5AB6-4DDC-0528-9CE9-A2A9C38588B5";
	setAttr ".t" -type "double3" 0 0.62731539785695034 0 ;
createNode mesh -n "pPyramidShape1" -p "pPyramid1";
	rename -uid "C1452868-4599-07B3-BCD7-5EA00802FF38";
	setAttr -k off ".v";
	setAttr -s 2 ".iog";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.49834081530570984 0.50745642185211182 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 102 ".uvst[0].uvsp[0:101]" -type "float2" 0.73935074 0.52090466
		 0.63840669 0.72280455 0.7420789 0.50613636 0.73240304 0.52337211 0.25451994 0.50899333
		 0.2568931 0.50620073 0.4924702 0.0075298408 0.25253743 0.50019467 0.74521899 0.50030237
		 0.63003004 0.5 0.25744778 0.25744775 0.25746995 0.24253003 0.37852412 0.50022691
		 0.37064376 0.50449812 0.38012072 0.50487614 0.6241256 0.52299303 0.57438314 0.72078532
		 0.61970162 0.50601935 0.61500746 0.52068484 0.62466449 0.5058732 0.62249988 0.50019461
		 0.5059889 0.5 0.49255973 0.49255973 0.49985692 0.50780004 0.49609828 0.50483763 0.5048638
		 0.51837736 0.50469261 0.73183823 0.42294335 0.75614721 0.4098061 0.75596482 0.3900426
		 0.75331235 0.43021354 0.74116039 0.40547881 0.7564947 0.41116422 0.75636429 0.54457414
		 0.75632948 0.43941268 0.73410004 0.496508 0.73467112 0.44308862 0.7432276 0.4351767
		 0.75341529 0.44716483 0.75190246 0.49343807 0.75580615 0.47311834 0.75532669 0.51011109
		 0.75585216 0.52560437 0.75623047 0.54860097 0.75320697 0.53906238 0.756042 0.50512952
		 0.74174577 0.4718416 0.75194192 0.48674265 0.74369383 0.49997634 0.74548125 0.74237311
		 0.24295112 0.74243444 0.2575312 0.74275494 0.25724503 0.507245 0.49275494 0.50724512
		 0.0072450498 0.49275494 0.49275494 0.25724506 0.25724503 0.50705957 0.49236152 0.49293965
		 0.49236014 0.50000006 0.020258853 0.27025887 0.24999999 0.49292508 0.007653004 0.25724503
		 0.24275494 0.25763848 0.25705957 0.62846994 0.50591326 0.50528526 0.50448591 0.39432356
		 0.75621545 0.43084192 0.74514735 0.50071895 0.75605768 0.48518425 0.75345379 0.43026441
		 0.75588065 0.45128092 0.75533593 0.63265973 0.71995288 0.2656275 0.52151316 0.37462351
		 0.51887196 0.42782581 0.7306962 0.36826336 0.72334433 0.38308761 0.51578224 0.49658725
		 0.51574767 0.56218052 0.72474772 0.50753188 0.0075661121 0.25756529 0.24246901 0.7297411
		 0.25 0.5 0.47974113 0.50752991 0.0075298329 0.74246931 0.2424693 0.74275494 0.24275495
		 0.74247158 0.25752842 0.50746071 0.49253929 0.49275491 0.0072451551 0.25654152 0.51308304
		 0.25 0.5 0.37128803 0.49999997 0.36871132 0.49999997 0.37561774 0.50653076 0.37677798
		 0.50972712 0.62767071 0.5 0.49915022 0.50000006 0.49633747 0.50000006 0.49985394
		 0.50971901 0.3806788 0.75650543 0.4357138 0.74134135 0.55111587 0.756248;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 72 ".vt[0:71]"  0.4778859 -0.1627776 -0.42947868 0.47647855 -0.20468161 -0.45536998
		 0.44665363 -0.20434493 -0.4861607 0.44786537 -0.16647175 -0.46830493 0.44646659 -0.24438405 -0.49180281
		 0.47609293 -0.24409285 -0.46217728 -0.50751299 -0.24695057 -0.46249965 -0.47812304 -0.24644929 -0.49180233
		 -0.42002445 -0.16984448 -0.47055516 -0.46748406 -0.20840389 -0.48763317 -0.49761498 -0.20702127 -0.45917344
		 -0.46937796 -0.17606843 -0.44802317 0.44777924 -0.16788679 0.46559832 0.44662049 -0.20475721 0.48543847
		 0.47646561 -0.20522887 0.4543677 0.47786266 -0.16328898 0.4236818 0.47609329 -0.24444774 0.46217337
		 0.4464733 -0.24479905 0.49180233 -0.4781591 -0.24636954 0.49180281 -0.50751269 -0.24681911 0.46252972
		 -0.46943802 -0.17610896 0.44511905 -0.49769208 -0.20705646 0.4585368 -0.46781456 -0.20835218 0.48698592
		 -0.42327476 -0.17080846 0.46786398 0.44663951 0.3005724 -0.087594315 0.41627181 0.30105352 -0.11519007
		 0.40672827 0.31366307 -0.072867379 0.46779206 0.24645811 -0.19275019 0.45230034 0.27613652 -0.16079128
		 0.47826418 0.27652264 -0.12448784 0.49641618 0.23932558 -0.16325204 0.19541898 0.270486 -0.1509687
		 0.23346904 0.28906006 -0.1224331 0.26461732 0.28720587 -0.15114143 0.22306605 0.26343095 -0.18142383
		 0.32721949 0.31331074 -0.067363419 0.31370109 0.30632865 -0.10771455 0.28586027 0.30647922 -0.076998144
		 0.41608676 0.30242169 0.060248226 0.44659781 0.30016321 0.033229604 0.40641555 0.31363189 0.019049497
		 0.49650642 0.24128121 0.11232504 0.47842479 0.27738631 0.072726697 0.45125306 0.27798367 0.10791494
		 0.46788502 0.24960041 0.14191 0.28597844 0.30664021 0.023602014 0.3141214 0.30649889 0.0543538
		 0.32733667 0.31328708 0.013674605 0.22498481 0.26581097 0.12934169 0.2652064 0.288297 0.098144084
		 0.23501682 0.28995502 0.068677738 0.19720058 0.27168268 0.098887689 0.47495446 -0.46408814 0.46288809
		 0.4463948 -0.46301687 0.4917348 0.43623975 -0.48012429 0.45194885 0.44758821 -0.44113249 0.49180233
		 0.47609329 -0.44113249 0.46329725 -0.50751269 -0.44113249 0.46329772 -0.4790076 -0.44113249 0.49180281
		 -0.46765921 -0.48012429 0.45194927 -0.47859845 -0.46408814 0.49066398 -0.50637388 -0.46408814 0.46288857
		 0.44717869 -0.46408814 -0.49066398 0.4760254 -0.46301687 -0.46210432 0.43623945 -0.48012429 -0.45194927
		 0.47609293 -0.44113249 -0.46329772 0.44758785 -0.44113249 -0.49180281 -0.50637418 -0.46408814 -0.46288809
		 -0.4778145 -0.46301687 -0.4917348 -0.46765944 -0.48012429 -0.45194885 -0.4790079 -0.44113249 -0.49180233
		 -0.50751299 -0.44113249 -0.46329725;
	setAttr -s 144 ".ed[0:143]"  0 3 1 3 27 0 27 30 1 30 0 0 1 0 0 0 15 1
		 15 14 0 14 1 0 2 1 0 1 5 0 3 2 0 2 9 0 9 8 0 8 3 1 4 5 1 5 65 0 65 66 0 66 4 0 2 4 0
		 4 7 1 16 5 1 6 7 1 7 70 0 70 71 0 71 6 0 6 19 1 6 10 0 10 9 0 9 7 0 8 11 1 11 31 0
		 31 34 1 34 8 0 11 10 0 10 21 0 21 20 0 20 11 1 12 15 1 15 41 0 41 44 1 44 12 0 13 12 0
		 12 23 1 23 22 0 22 13 0 14 13 0 13 17 0 16 14 0 16 17 1 17 55 0 55 56 0 56 16 0 18 17 1
		 18 19 1 19 57 0 57 58 0 58 18 0 18 22 0 22 21 0 21 19 0 20 23 1 23 48 1 48 51 1 51 20 1
		 24 26 1 26 40 1 40 39 1 39 24 1 25 24 1 24 29 1 29 28 1 28 25 1 26 25 1 25 36 1 36 35 1
		 35 26 1 28 27 0 27 34 1 34 33 1 33 28 1 30 29 0 29 42 1 42 41 0 41 30 1 32 31 1 31 51 1
		 51 50 1 50 32 1 33 32 1 32 37 1 37 36 1 36 33 1 35 37 1 37 45 1 45 47 1 47 35 1 38 40 1
		 40 47 1 47 46 1 46 38 1 39 38 1 38 43 1 43 42 1 42 39 1 44 43 0 43 49 1 49 48 1 48 44 1
		 46 45 1 45 50 1 50 49 1 49 46 1 52 54 1 54 64 1 64 63 1 63 52 0 53 52 0 52 56 0 54 53 1
		 53 60 0 60 59 1 59 54 1 53 55 0 55 58 1 65 56 1 57 71 1 71 67 0 57 61 0 61 60 0 60 58 1
		 59 61 1 61 67 0 67 69 1 69 59 1 62 64 1 64 69 1 69 68 1 68 62 0 63 62 0 62 66 0 65 63 1
		 70 66 1 68 67 0 70 68 1;
	setAttr -s 74 -ch 288 ".fc[0:73]" -type "polyFaces" 
		f 4 0 1 2 3
		mu 0 4 3 0 1 71
		f 4 4 5 6 7
		mu 0 4 2 3 15 63
		f 4 8 9 -15 -19
		mu 0 4 5 4 90 7
		f 4 10 11 12 13
		mu 0 4 72 5 13 73
		f 4 14 15 16 17
		mu 0 4 6 83 53 88
		f 4 18 19 -29 -12
		mu 0 4 5 7 92 13
		f 4 20 -10 -8 -48
		mu 0 4 9 8 2 63
		f 4 21 22 23 24
		mu 0 4 10 11 61 55
		f 4 25 -60 -35 -27
		mu 0 4 12 97 24 14
		f 4 26 27 28 -22
		mu 0 4 91 93 13 92
		f 4 29 30 31 32
		mu 0 4 73 76 34 74
		f 4 33 34 35 36
		mu 0 4 76 14 24 77
		f 4 37 38 39 40
		mu 0 4 18 15 16 78
		f 4 41 42 43 44
		mu 0 4 17 18 25 64
		f 4 47 45 46 -49
		mu 0 4 9 63 19 95
		f 4 48 49 50 51
		mu 0 4 84 86 51 85
		f 4 52 -47 -45 -58
		mu 0 4 21 20 17 64
		f 4 53 54 55 56
		mu 0 4 87 22 54 52
		f 4 57 58 59 -54
		mu 0 4 96 23 24 97
		f 4 60 61 62 63
		mu 0 4 77 25 26 35
		f 4 64 65 66 67
		mu 0 4 27 69 39 67
		f 4 68 69 70 71
		mu 0 4 28 27 32 65
		f 4 72 73 74 75
		mu 0 4 69 28 37 70
		f 4 76 77 78 79
		mu 0 4 65 29 30 66
		f 4 80 81 82 83
		mu 0 4 31 32 42 33
		f 4 84 85 86 87
		mu 0 4 36 34 35 47
		f 4 88 89 90 91
		mu 0 4 66 36 38 37
		f 4 92 93 94 95
		mu 0 4 70 38 46 40
		f 4 96 97 98 99
		mu 0 4 41 39 40 68
		f 4 100 101 102 103
		mu 0 4 67 41 44 42
		f 4 104 105 106 107
		mu 0 4 43 44 48 45
		f 4 108 109 110 111
		mu 0 4 68 46 47 48
		f 4 112 113 114 115
		mu 0 4 49 81 58 79
		f 4 116 117 -51 -123
		mu 0 4 50 49 85 51
		f 4 118 119 120 121
		mu 0 4 81 50 56 82
		f 4 122 123 -130 -120
		mu 0 4 50 51 52 56
		f 4 124 -118 -116 -141
		mu 0 4 53 85 49 79
		f 4 125 126 -132 -128
		mu 0 4 54 55 62 57
		f 4 127 128 129 -56
		mu 0 4 54 57 56 52
		f 4 130 131 132 133
		mu 0 4 82 57 62 59
		f 4 134 135 136 137
		mu 0 4 60 58 59 80
		f 4 140 138 139 -17
		mu 0 4 53 79 60 88
		f 4 141 -140 -138 -144
		mu 0 4 61 88 60 80
		f 4 143 142 -127 -24
		mu 0 4 61 80 62 55
		f 4 -72 -80 -92 -74
		mu 0 4 28 65 66 37
		f 4 -70 -68 -104 -82
		mu 0 4 32 27 67 42
		f 4 -102 -100 -112 -106
		mu 0 4 44 41 68 48
		f 4 -90 -88 -110 -94
		mu 0 4 38 36 47 46
		f 4 -66 -76 -96 -98
		mu 0 4 39 69 70 40
		f 4 -6 -4 -84 -39
		mu 0 4 15 3 71 16
		f 4 -14 -33 -78 -2
		mu 0 4 72 73 74 75
		f 4 -37 -64 -86 -31
		mu 0 4 76 77 35 34
		f 4 -43 -41 -108 -62
		mu 0 4 25 18 78 26
		f 4 -114 -122 -134 -136
		mu 0 4 58 81 82 59
		f 4 -21 -52 -125 -16
		mu 0 4 83 84 85 53
		f 4 -53 -57 -124 -50
		mu 0 4 86 87 52 51
		f 4 -26 -25 -126 -55
		mu 0 4 22 10 55 54
		f 4 -20 -18 -142 -23
		mu 0 4 11 6 88 61
		f 4 -5 -9 -11 -1
		mu 0 4 89 4 5 72
		f 4 -13 -28 -34 -30
		mu 0 4 73 13 93 94
		f 4 -42 -46 -7 -38
		mu 0 4 18 19 63 15
		f 4 -36 -59 -44 -61
		mu 0 4 77 24 23 98
		f 3 -69 -73 -65
		mu 0 3 27 28 69
		f 4 -77 -71 -81 -3
		mu 0 4 29 65 32 99
		f 4 -85 -89 -79 -32
		mu 0 4 100 36 66 30
		f 3 -75 -91 -93
		mu 0 3 70 37 38
		f 3 -101 -67 -97
		mu 0 3 41 67 39
		f 4 -83 -103 -105 -40
		mu 0 4 33 42 44 101
		f 3 -109 -99 -95
		mu 0 3 46 68 40
		f 4 -107 -111 -87 -63
		mu 0 4 45 48 47 35
		f 3 -117 -119 -113
		mu 0 3 49 50 81
		f 3 -121 -129 -131
		mu 0 3 82 56 57
		f 3 -139 -115 -135
		mu 0 3 60 79 58
		f 3 -143 -137 -133
		mu 0 3 62 80 59;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "polySurface1";
	rename -uid "E08B8581-410A-3076-6E5D-54B1A6F3CBC6";
createNode mesh -n "polySurfaceShape1" -p "polySurface1";
	rename -uid "0DC66B1C-4311-633D-71CD-46923D2F7149";
	setAttr -k off ".v";
	setAttr -s 2 ".iog";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 32 ".pt";
	setAttr ".pt[0]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[1]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[2]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[3]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[4]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[5]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[6]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[7]" -type "float3" 1.1920929e-007 2.3841858e-007 -2.3841858e-007 ;
	setAttr ".pt[17]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[18]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[19]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[20]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[21]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[22]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[23]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[24]" -type "float3" 0 1.1920929e-007 -1.1920929e-007 ;
	setAttr ".pt[153]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[154]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[155]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[156]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[157]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[158]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[159]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[160]" -type "float3" 0 -1.1920929e-007 1.1920929e-007 ;
	setAttr ".pt[170]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[171]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[172]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[173]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[174]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[175]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[176]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".pt[177]" -type "float3" 0 0 2.9802322e-008 ;
	setAttr ".bw" 3;
	setAttr ".dr" 1;
createNode transform -n "curve1";
	rename -uid "8440E6E6-4019-17EF-81F8-34BD5C726555";
	setAttr ".t" -type "double3" 5.9878090295186297 6.5036301826659724 4.9481099677454079 ;
	setAttr ".r" -type "double3" -22.564793924772449 15.050473412768632 -46.639156629215542 ;
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "36385764-4187-FEDB-1019-61A3AADCF36E";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 6 0 no 3
		11 0 0 0 1 2 3 4 5 6 6 6
		9
		7.0183655484471847 0 -28.524178976800798
		1.1240047737882133 0 -29.365422992336747
		-10.664716775529683 0 -31.047911023408453
		-15.431954746716267 0 -14.298226603079708
		-0.060033133620199144 -0.056848832881084888 -1.2893019075581467
		-2.454297054151048 0 13.320153902868684
		-20.70921889697107 0 14.73856437111837
		-28.492713137502385 0 -0.2242405144345252
		-34.732407065327592 1.674078509465474 -6.0405752026920716
		;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "3AFBD4AC-441D-EE89-50CE-56A3A463744D";
	setAttr -s 2 ".lnk";
	setAttr -s 2 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "5D786545-4AB2-6616-6037-34B1FB2B2181";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "395F4C08-49FB-ECE0-4E53-81A513FD1AF3";
createNode displayLayerManager -n "layerManager";
	rename -uid "48AE2646-44DE-CBE1-B6A1-4CACBA0B08C5";
createNode displayLayer -n "defaultLayer";
	rename -uid "2B11D7DF-4376-5669-0665-06858ABAA145";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "47061ACB-4939-0EC6-9AA4-4B83EBE9A9B5";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "92157220-46A3-3144-4800-84B3EDF69510";
	setAttr ".g" yes;
createNode clonerMulti -n "clonerMulti1";
	rename -uid "EC58BA4F-4791-83F5-A92E-ADB619C22129";
	setAttr -s 2 ".inMesh";
	setAttr ".instanceType" 4;
	setAttr ".patternType" 1;
	setAttr ".instanceX" 30;
	setAttr ".instanceY" 3;
	setAttr ".offsetX" 0;
	setAttr ".offsetY" 0;
	setAttr ".smoothNormals" yes;
createNode script -n "uiConfigurationScriptNode";
	rename -uid "E204615B-4EA4-EE5C-6E1D-5EA0FEA02C4C";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n"
		+ "            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n"
		+ "        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n"
		+ "                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n"
		+ "                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n"
		+ "            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n"
		+ "            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n"
		+ "            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n"
		+ "                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n"
		+ "                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n"
		+ "                -width 1\n                -height 1\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n"
		+ "            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n"
		+ "            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n"
		+ "                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 0\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"vp2Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n"
		+ "                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 2498\n                -height 1093\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n"
		+ "            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n"
		+ "            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 2498\n            -height 1093\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n"
		+ "            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAssignedMaterials 0\n                -showReferenceNodes 1\n                -showReferenceMembers 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n"
		+ "                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n"
		+ "                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n"
		+ "            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n"
		+ "            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n"
		+ "                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n"
		+ "                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n"
		+ "                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n"
		+ "                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n"
		+ "                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n"
		+ "                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n"
		+ "            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n"
		+ "                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n"
		+ "                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n"
		+ "                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n"
		+ "                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n"
		+ "                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n"
		+ "\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n"
		+ "                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n"
		+ "                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tshapePanel -unParent -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tposePanel -unParent -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"profilerPanel\" -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"contentBrowserPanel\" -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n"
		+ "                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab 0\n                -editorMode \"default\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n"
		+ "                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab 0\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n"
		+ "\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 2498\\n    -height 1093\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 2498\\n    -height 1093\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 10 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "4317A582-4046-F828-146F-34BF08893A9D";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode nodeGraphEditorInfo -n "MayaNodeEditorSavedTabsInfo";
	rename -uid "62DC32EF-43AD-7FCA-ECF1-9F977951433B";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" 993.70590210481305 -458.33331512080377 ;
	setAttr ".tgi[0].vh" -type "double2" 2287.2463484742389 419.04760239616337 ;
	setAttr -s 11 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 1153.5714111328125;
	setAttr ".tgi[0].ni[0].y" -163.09523010253906;
	setAttr ".tgi[0].ni[0].nvs" 18304;
	setAttr ".tgi[0].ni[1].x" -170;
	setAttr ".tgi[0].ni[1].y" -22.857143402099609;
	setAttr ".tgi[0].ni[1].nvs" 18304;
	setAttr ".tgi[0].ni[2].x" 1955.7142333984375;
	setAttr ".tgi[0].ni[2].y" -51.428569793701172;
	setAttr ".tgi[0].ni[2].nvs" 18304;
	setAttr ".tgi[0].ni[3].x" 1700;
	setAttr ".tgi[0].ni[3].y" 871.4285888671875;
	setAttr ".tgi[0].ni[3].nvs" 18306;
	setAttr ".tgi[0].ni[4].x" 2314.28564453125;
	setAttr ".tgi[0].ni[4].y" 822.85711669921875;
	setAttr ".tgi[0].ni[4].nvs" 18306;
	setAttr ".tgi[0].ni[5].x" 625.71429443359375;
	setAttr ".tgi[0].ni[5].y" 285.71429443359375;
	setAttr ".tgi[0].ni[5].nvs" 18304;
	setAttr ".tgi[0].ni[6].x" 645.71429443359375;
	setAttr ".tgi[0].ni[6].y" 155.71427917480469;
	setAttr ".tgi[0].ni[6].nvs" 18304;
	setAttr ".tgi[0].ni[7].x" 2007.142822265625;
	setAttr ".tgi[0].ni[7].y" 394.28570556640625;
	setAttr ".tgi[0].ni[7].nvs" 18306;
	setAttr ".tgi[0].ni[8].x" 388.57144165039063;
	setAttr ".tgi[0].ni[8].y" -277.14285278320312;
	setAttr ".tgi[0].ni[8].nvs" 18304;
	setAttr ".tgi[0].ni[9].x" 1700;
	setAttr ".tgi[0].ni[9].y" 265.71429443359375;
	setAttr ".tgi[0].ni[9].nvs" 18306;
	setAttr ".tgi[0].ni[10].x" 1700;
	setAttr ".tgi[0].ni[10].y" -340;
	setAttr ".tgi[0].ni[10].nvs" 18306;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".msaa" yes;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 2 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 4 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :initialShadingGroup;
	setAttr -s 3 ".dsm";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "clonerMulti1.outMesh" "polySurfaceShape1.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "pCubeShape1.w" "clonerMulti1.inMesh[0]";
connectAttr "pPyramidShape1.w" "clonerMulti1.inMesh[1]";
connectAttr "curveShape1.ws" "clonerMulti1.inCurve";
connectAttr "polySurface1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[0].dn";
connectAttr "pCube1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[1].dn";
connectAttr "curve1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[2].dn";
connectAttr "curveShape1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[3].dn";
connectAttr "polySurfaceShape1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[4].dn"
		;
connectAttr "sceneConfigurationScriptNode.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[5].dn"
		;
connectAttr "uiConfigurationScriptNode.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[6].dn"
		;
connectAttr "clonerMulti1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[7].dn";
connectAttr "pPyramid1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[8].dn";
connectAttr "pPyramidShape1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[9].dn";
connectAttr "pCubeShape1.msg" "MayaNodeEditorSavedTabsInfo.tgi[0].ni[10].dn";
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "pCubeShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pPyramidShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "polySurfaceShape1.iog" ":initialShadingGroup.dsm" -na;
// End of inst_simple.ma
