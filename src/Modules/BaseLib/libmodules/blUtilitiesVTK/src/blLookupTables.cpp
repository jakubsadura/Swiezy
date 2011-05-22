/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blLookupTables.h"
#include "vtkLookupTable.h"

std::map<blLookupTables::LUT_TYPE, std::string> blLookupTables::m_LutName;

blLookupTables::blLookupTables( void )
{
	BuildLutNames( );
}

blLookupTables::~blLookupTables( void )
{
}


vtkLookupTable * blLookupTables::BuildLookupTable(
						LUT_TYPE type,
						double exp,
						COLOR_TYPE color )
{
	vtkLookupTable * lookupTablePt = 0;
	switch(type)
	{
		case LUT_TYPE_NON_PERCEP_LIN_GRAY:lookupTablePt = BuildLutNonPercepLinGray();break;
		case LUT_TYPE_PERCEP_LIN_GRAY:lookupTablePt = BuildLutPercepLinGray( exp );break;
		case LUT_TYPE_PERCEP_LIN_RAINBOW:lookupTablePt = BuildLutPercepLinRainbow();break;
		case LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED:lookupTablePt = BuildLutPercepLinRainbowInverted();break;
		case LUT_TYPE_PERCEP_LIN_HEATED_OBJECT:lookupTablePt = BuildLutPercepLinHeatedObject();break;
		case LUT_TYPE_PERCEP_LIN_MAGENTA:lookupTablePt = BuildLutPercepLinMagenta();break;
		case LUT_TYPE_OPTIMAL:lookupTablePt = BuildLutOptimal();break;
		case LUT_TYPE_PERCEP_LIN_OPTIMAL:lookupTablePt = BuildLutPercepLinOptimal();break;
		case LUT_TYPE_BLUE_TO_CYAN:lookupTablePt = BuildLutBlueToCyan();break;
		case LUT_TYPE_BLUE_TO_YELLOW:lookupTablePt = BuildLutBlueToYellow();break;
		case LUT_TYPE_WHITE_TO_RED:lookupTablePt = BuildLutWhiteToRed();break;
		case LUT_TYPE_BLACK_WHITE:lookupTablePt = BuildLookupTableBLACKWHITE();break; 
		case LUT_TYPE_WHITE_BLACK:lookupTablePt = BuildLookupTableWHITEBLACK();break; 
		case LUT_TYPE_HOT_METAL:lookupTablePt = BuildLookupTableHOTMETAL();break; 
		case LUT_TYPE_ICE:lookupTablePt = BuildLookupTableICE();break; 
		case LUT_TYPE_HOT:lookupTablePt = BuildLookupTableHOT();break; 
		case LUT_TYPE_GE:lookupTablePt = BuildLookupTableGE();break;
		case LUT_TYPE_PRIMARY_COLOR:lookupTablePt = BuildLookupTablePRIMARYCOLOR(color);break;
		case LUT_TYPE_16_COLORS:lookupTablePt = BuildLookupTable16Colors();break;
		case LUT_TYPE_RAINBOW_HUE:lookupTablePt = BuildLookupTableRaibowHue();break;
		case LUT_TYPE_RAINBOW_HUE_INVERTED:lookupTablePt = BuildLookupTableRaibowHueInverted();break;
		case LUT_TYPE_RAINBOW_GE:lookupTablePt = BuildLookupTableRainbowGE();break;
		case LUT_TYPE_SPECTRUM_SIEMENS:lookupTablePt = BuildLookupTableSpectrumSiemens();break;
		case LUT_TYPE_MITK_GRAY:lookupTablePt = BuildLutMITKGray( );break;
		case LUT_TYPE_RED_BLUE:lookupTablePt = BuildLutRedBlue( );break;
	}
	return lookupTablePt;
}


vtkLookupTable * blLookupTables::BuildLutNonPercepLinGray()
{
	if( m_lutTable[ LUT_TYPE_NON_PERCEP_LIN_GRAY ] == NULL )
	{
		m_lutTable[ LUT_TYPE_NON_PERCEP_LIN_GRAY ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_NON_PERCEP_LIN_GRAY ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_NON_PERCEP_LIN_GRAY ]->Build();
		for (int i=0; i<256; i++) {
			m_lutTable[ LUT_TYPE_NON_PERCEP_LIN_GRAY ]->SetTableValue(
				i, 
				1.0-(255-(double)i)/255.0, 
				1.0-(255-(double)i)/255.0, 
				1.0-(255-(double)i)/255.0, 
				1.0);
		}	
	}

	return m_lutTable[ LUT_TYPE_NON_PERCEP_LIN_GRAY ];
}

vtkLookupTable * blLookupTables::BuildLutPercepLinGray( double exp )
{
	if( m_lutTable[ LUT_TYPE_PERCEP_LIN_GRAY ] == NULL )
	{
		m_lutTable[ LUT_TYPE_PERCEP_LIN_GRAY ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_PERCEP_LIN_GRAY ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_GRAY ]->Build();

		double incr = 1.0/255.0;

		for (int i=0; i<256; i++) {
			m_lutTable[ LUT_TYPE_PERCEP_LIN_GRAY ]->SetTableValue(
				i, 
				pow((double)i*incr, exp), 
				pow((double)i*incr, exp), 
				pow((double)i*incr, exp), 
				1.0);
		}
	}

	return m_lutTable[ LUT_TYPE_PERCEP_LIN_GRAY ];
}


vtkLookupTable * blLookupTables::BuildLookupTableRainbowGE()
{
	if( m_lutTable[ LUT_TYPE_RAINBOW_GE ] == NULL )
	{
		m_lutTable[ LUT_TYPE_RAINBOW_GE ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->Build();

		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(0	,0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(1	,0,	0,	0.02,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(2	,0,	0,	0.03,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(3	,0,	0,	0.05,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(4	,0,	0,	0.06,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(5	,0,	0,	0.08,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(6	,0,	0,	0.10,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(7	,0,	0,	0.11,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(8	,0,	0,	0.13,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(9	,0,	0,	0.14,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(10	,0,	0,	0.16,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(11	,0,	0,	0.17,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(12	,0,	0,	0.19,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(13	,0,	0,	0.21,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(14	,0,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(15	,0,	0,	0.24,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(16	,0,	0,	0.25,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(17	,0,	0,	0.27,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(18	,0,	0,	0.29,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(19	,0,	0,	0.30,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(20	,0,	0,	0.32,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(21	,0,	0,	0.33,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(22	,0,	0,	0.35,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(23	,0,	0,	0.37,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(24	,0,	0,	0.38,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(25	,0,	0,	0.40,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(26	,0,	0,	0.41,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(27	,0,	0,	0.43,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(28	,0,	0,	0.44,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(29	,0,	0.,	0.46,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(30	,0,	0,	0.48,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(31	,0,	0,	0.49,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(32	,0,	0,	0.51,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(33	,0,	0,	0.52,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(34	,0,	0,	0.54,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(35	,0,	0,	0.56,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(36	,0,	0,	0.57,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(37	,0,	0,	0.59,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(38	,0,	0,	0.60,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(39	,0,	0,	0.62,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(40	,0,	0,	0.63,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(41	,0,	0,	0.65,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(42	,0,	0,	0.67,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(43	,0,	0,	0.68,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(44	,0,	0,	0.70,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(45	,0,	0,	0.71,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(46	,0,	0,	0.73,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(47	,0,	0,	0.75,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(48	,0,	0,	0.75,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(49	,0,	0.02,	0.73,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(50	,0,	0.03,	0.71,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(51	,0,	0.05,	0.70,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(52	,0,	0.06,	0.68,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(53	,0,	0.08,	0.67,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(54	,0,	0.10,	0.65,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(55	,0,	0.11,	0.63,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(56	,0,	0.13,	0.62,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(57	,0,	0.14,	0.60,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(58	,0,	0.16,	0.59,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(59	,0,	0.17,	0.57,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(60	,0,	0.19,	0.56,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(61	,0,	0.21,	0.54,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(62	,0,	0.22,	0.52,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(63	,0,	0.24,	0.51,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(64	,0,	0.25,	0.49,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(65	,0,	0.27,	0.48,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(66	,0,	0.29,	0.46,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(67	,0,	0.30,	0.44,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(68	,0,	0.32,	0.43,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(69	,0,	0.33,	0.41,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(70	,0,	0.35,	0.40,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(71	,0,	0.37,	0.38,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(72	,0,	0.38,	0.37,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(73	,0,	0.40,	0.35,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(74	,0,	0.41,	0.33,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(75	,0,	0.43,	0.32,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(76	,0,	0.44,	0.30,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(77	,0,	0.46,	0.29,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(78	,0,	0.48,	0.27,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(79	,0,	0.49,	0.25,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(80	,0,	0.51,	0.24,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(81	,0,	0.52,	0.22,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(82	,0,	0.54,	0.21,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(83	,0,	0.56,	0.19,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(84	,0,	0.57,	0.17,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(85	,0,	0.59,	0.16,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(86	,0,	0.60,	0.14,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(87	,0,	0.62,	0.13,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(88	,0,	0.63,	0.11,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(89	,0,	0.65,	0.10,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(90	,0,	0.67,	0.08,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(91	,0,	0.68,	0.06,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(92	,0,	0.70,	0.05,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(93	,0,	0.71,	0.03,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(94	,0,	0.73,	0.02,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(95	,0,	0.75,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(96	,0,	0.76,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(97	,0.02,	0.78,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(98	,0.03,	0.79,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(99	,0.05,	0.81,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(100	,0.06,	0.83,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(101	,0.08,	0.84,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(102	,0.10,	0.86,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(103	,0.11,	0.87,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(104	,0.14,	0.89,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(105	,0.16,	0.90,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(106	,0.17,	0.92,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(107	,0.18,	0.94,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(108	,0.19,	0.95,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(109	,0.21,	0.97,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(110	,0.22,	0.98,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(111	,0.24,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(112	,0.25,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(113	,0.27,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(114	,0.29,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(115	,0.30,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(116	,0.32,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(117	,0.33,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(118	,0.35,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(119	,0.37,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(120	,0.38,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(121	,0.40,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(122	,0.41,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(123	,0.43,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(124	,0.44,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(125	,0.46,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(126	,0.48,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(127	,0.49,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(128	,0.51,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(129	,0.52,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(130	,0.54,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(131	,0.56,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(132	,0.57,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(133	,0.59,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(134	,0.60,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(135	,0.62,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(136	,0.63,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(137	,0.65,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(138	,0.67,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(139	,0.68,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(140	,0.70,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(141	,0.71,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(142	,0.73,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(143	,0.75,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(144	,0.76,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(145	,0.78,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(146	,0.79,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(147	,0.81,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(148	,0.83,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(149	,0.84,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(150	,0.86,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(151	,0.87,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(152	,0.89,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(153	,0.90,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(154	,0.92,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(155	,0.94,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(156	,0.95,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(157	,0.97,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(158	,0.98,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(159	,1,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(160	,1,	1,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(161	,1,	0.98,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(162	,1,	0.97,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(163	,1,	0.95,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(164	,1,	0.94,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(165	,1,	0.92,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(166	,1,	0.90,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(167	,1,	0.89,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(168	,1,	0.87,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(169	,1,	0.86,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(170	,1,	0.84,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(171	,1,	0.83,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(172	,1,	0.81,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(173	,1,	0.79,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(174	,1,	0.78,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(175	,1,	0.76,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(176	,1,	0.75,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(177	,1,	0.73,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(178	,1,	0.71,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(179	,1,	0.70,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(180	,1,	0.68,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(181	,1,	0.67,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(182	,1,	0.65,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(183	,1,	0.63,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(184	,1,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(185	,1,	0.60,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(186	,1,	0.59,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(187	,1,	0.57,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(188	,1,	0.56,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(189	,1,	0.54,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(190	,1,	0.52,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(191	,1,	0.51,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(192	,1,	0.49,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(193	,1,	0.48,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(194	,1,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(195	,1,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(196	,1,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(197	,1,	0.41,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(198	,1,	0.40,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(199	,1,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(200	,1,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(201	,1,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(202	,1,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(203	,1,	0.32,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(204	,1,	0.30,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(205	,1,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(206	,1,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(207	,1,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(208	,1,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(209	,1,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(210	,1,	0.21,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(211	,1,	0.19,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(212	,1,	0.17,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(213	,1,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(214	,1,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(215	,1,	0.13,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(216	,1,	0.11,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(217	,1,	0.10,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(218	,1,	0.08,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(219	,1,	0.06,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(220	,1,	0.05,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(221	,1,	0.03,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(222	,1,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(223	,1,	0,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(224	,1,	0,	0,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(225	,1,	0.03,	0.03,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(226	,1,	0.06,	0.06,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(227	,1,	0.10,	0.10,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(228	,1,	0.13,	0.13,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(229	,1,	0.16,	0.16,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(230	,1,	0.19,	0.19,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(231	,1,	0.22,	0.22,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(232	,1,	0.25,	0.25,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(233	,1,	0.29,	0.29,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(234	,1,	0.32,	0.32,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(235	,1,	0.35,	0.35,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(236	,1,	0.38,	0.38,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(237	,1,	0.41,	0.41,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(238	,1,	0.44,	0.44,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(239	,1,	0.48,	0.48,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(240	,1,	0.51,	0.51,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(241	,1,	0.54,	0.54,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(242	,1,	0.57,	0.57,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(243	,1,	0.60,	0.60,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(244	,1,	0.63,	0.63,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(245	,1,	0.67,	0.67,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(246	,1,	0.70,	0.70,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(247	,1,	0.73,	0.73,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(248	,1,	0.76,	0.76,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(249	,1,	0.79,	0.79,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(250	,1,	0.83,	0.83,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(251	,1,	0.86,	0.86,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(252	,1,	0.89,	0.89,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(253	,1,	0.92,	0.92,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(254	,1,	0.95,	0.95,	1);
		m_lutTable[ LUT_TYPE_RAINBOW_GE ]->SetTableValue(255	,1,	1,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_RAINBOW_GE ];
}

vtkLookupTable * blLookupTables::BuildLookupTableSpectrumSiemens()
{
	if( m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ] == NULL )
	{
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->Build();

		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(0	,0.08,	0,	0.06,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(1	,0.10,	0,	0.07,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(2	,0.12,	0,	0.08,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(3	,0.14,	0,	0.10,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(4	,0.16,	0,	0.11,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(5	,0.18,	0,	0.13,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(6	,0.20,	0,	0.14,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(7	,0.22,	0,	0.15,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(8	,0.24,	0,	0.16,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(9	,0.25,	0,	0.18,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(10	,0.27,	0,	0.19,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(11	,0.29,	0,	0.21,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(12	,0.31,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(13	,0.33,	0,	0.24,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(14	,0.35,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(15	,0.35,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(16	,0.36,	0,	0.21,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(17	,0.38,	0,	0.20,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(18	,0.39,	0,	0.20,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(19	,0.40,	0,	0.19,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(20	,0.41,	0,	0.18,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(21	,0.43,	0,	0.17,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(22	,0.45,	0,	0.15,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(23	,0.46,	0,	0.15,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(24	,0.47,	0,	0.14,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(25	,0.48,	0,	0.13,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(26	,0.49,	0,	0.13,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(27	,0.51,	0,	0.12,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(28	,0.51,	0,	0.11,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(29	,0.53,	0.,	0.11,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(30	,0.55,	0,	0.10,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(31	,0.56,	0,	0.09,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(32	,0.58,	0,	0.09,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(33	,0.60,	0,	0.08,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(34	,0.61,	0,	0.07,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(35	,0.63,	0,	0.07,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(36	,0.65,	0,	0.06,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(37	,0.66,	0,	0.05,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(38	,0.68,	0,	0.05,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(39	,0.70,	0,	0.04,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(40	,0.71,	0,	0.04,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(41	,0.73,	0,	0.03,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(42	,0.75,	0,	0.03,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(43	,0.78,	0,	0.02,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(44	,0.80,	0,	0.01,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(45	,0.82,	0,	0.01,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(46	,0.82,	0.01,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(47	,0.80,	0.03,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(48	,0.78,	0.04,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(49	,0.76,	0.06,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(50	,0.75,	0.08,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(51	,0.73,	0.10,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(52	,0.71,	0.12,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(53	,0.69,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(54	,0.67,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(55	,0.65,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(56	,0.63,	0.20,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(57	,0.61,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(58	,0.59,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(59	,0.57,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(60	,0.55,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(61	,0.53,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(62	,0.51,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(63	,0.50,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(64	,0.47,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(65	,0.47,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(66	,0.46,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(67	,0.45,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(68	,0.44,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(69	,0.43,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(70	,0.42,	0.40,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(71	,0.41,	0.40,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(72	,0.40,	0.41,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(73	,0.39,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(74	,0.38,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(75	,0.37,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(76	,0.36,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(77	,0.35,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(78	,0.34,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(79	,0.33,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(80	,0.33,	0.47,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(81	,0.31,	0.49,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(82	,0.31,	0.50,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(83	,0.30,	0.51,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(84	,0.29,	0.52,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(85	,0.27,	0.53,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(86	,0.26,	0.56,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(87	,0.25,	0.57,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(88	,0.24,	0.58,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(89	,0.23,	0.59,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(90	,0.22,	0.60,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(91	,0.21,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(92	,0.20,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(93	,0.20,	0.64,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(94	,0.18,	0.65,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(95	,0.18,	0.66,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(96	,0.17,	0.67,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(97	,0.16,	0.69,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(98	,0.15,	0.71,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(99	,0.14,	0.73,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(100	,0.13,	0.75,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(101	,0.12,	0.77,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(102	,0.11,	0.79,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(103	,0.09,	0.81,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(104	,0.09,	0.83,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(105	,0.08,	0.85,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(106	,0.07,	0.87,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(107	,0.05,	0.90,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(108	,0.04,	0.93,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(109	,0.03,	0.95,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(110	,0.02,	0.96,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(111	,0,	0.98,	0,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(112	,0,	1,	0.01,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(113	,0,	0.99,	0.05,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(114	,0,	0.98,	0.09,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(115	,0,	0.96,	0.12,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(116	,0,	0.96,	0.16,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(117	,0,	0.95,	0.20,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(118	,0,	0.94,	0.24,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(119	,0,	0.93,	0.27,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(120	,0,	0.92,	0.31,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(121	,0,	0.91,	0.35,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(122	,0,	0.90,	0.39,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(123	,0,	0.89,	0.43,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(124	,0,	0.88,	0.47,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(125	,0,	0.87,	0.51,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(126	,0,	0.86,	0.55,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(127	,0,	0.85,	0.58,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(128	,0,	0.83,	0.66,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(129	,0,	0.84,	0.68,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(130	,0,	0.84,	0.70,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(131	,0,	0.84,	0.72,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(132	,0,	0.85,	0.74,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(133	,0,	0.85,	0.76,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(134	,0,	0.85,	0.78,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(135	,0,	0.85,	0.80,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(136	,0,	0.86,	0.82,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(137	,0,	0.87,	0.84,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(138	,0,	0.87,	0.85,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(139	,0,	0.87,	0.87,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(140	,0,	0.88,	0.89,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(141	,0,	0.88,	0.91,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(142	,0,	0.89,	0.93,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(143	,0,	0.89,	0.95,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(144	,0,	0.89,	0.97,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(145	,0,	0.90,	0.99,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(146	,0,	0.89,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(147	,0,	0.88,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(148	,0,	0.86,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(149	,0,	0.85,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(150	,0,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(151	,0,	0.81,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(152	,0,	0.80,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(153	,0,	0.78,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(154	,0,	0.77,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(155	,0,	0.76,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(156	,0,	0.74,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(157	,0,	0.73,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(158	,0,	0.72,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(159	,0,	0.70,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(160	,0,	0.69,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(161	,0,	0.67,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(162	,0,	0.66,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(163	,0,	0.65,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(164	,0,	0.63,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(165	,0,	0.62,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(166	,0,	0.61,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(167	,0,	0.59,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(168	,0,	0.58,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(169	,0,	0.56,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(170	,0,	0.55,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(171	,0,	0.53,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(172	,0,	0.51,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(173	,0,	0.50,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(174	,0,	0.49,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(175	,0,	0.47,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(176	,0,	0.45,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(177	,0,	0.44,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(178	,0,	0.43,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(179	,0,	0.42,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(180	,0,	0.41,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(181	,0,	0.40,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(182	,0,	0.39,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(183	,0,	0.38,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(184	,0,	0.37,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(185	,0,	0.36,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(186	,0,	0.35,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(187	,0,	0.35,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(188	,0,	0.33,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(189	,0,	0.33,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(190	,0,	0.32,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(191	,0,	0.31,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(192	,0,	0.29,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(193	,0,	0.27,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(194	,0,	0.27,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(195	,0,	0.25,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(196	,0,	0.24,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(197	,0,	0.22,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(198	,0,	0.21,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(199	,0,	0.19,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(200	,0,	0.18,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(201	,0,	0.16,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(202	,0,	0.15,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(203	,0,	0.13,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(204	,0,	0.11,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(205	,0,	0.10,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(206	,0,	0.09,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(207	,0,	0.07,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(208	,0,	0.05,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(209	,0,	0.04,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(210	,0,	0.02,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(211	,0,	0.01,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(212	,0.02,	0.01,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(213	,0.06,	0.05,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(214	,0.13,	0.11,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(215	,0.17,	0.15,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(216	,0.21,	0.18,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(217	,0.25,	0.21,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(218	,0.29,	0.24,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(219	,0.33,	0.27,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(220	,0.36,	0.31,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(221	,0.40,	0.34,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(222	,0.44,	0.37,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(223	,0.48,	0.41,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(224	,0.52,	0.44,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(225	,0.56,	0.47,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(226	,0.59,	0.50,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(227	,0.63,	0.54,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(228	,0.67,	0.56,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(229	,0.68,	0.57,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(230	,0.69,	0.58,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(231	,0.71,	0.60,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(232	,0.72,	0.60,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(233	,0.73,	0.61,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(234	,0.75,	0.62,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(235	,0.77,	0.64,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(236	,0.79,	0.65,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(237	,0.80,	0.66,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(238	,0.82,	0.67,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(239	,0.83,	0.68,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(240	,0.85,	0.69,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(241	,0.85,	0.70,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(242	,0.87,	0.71,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(243	,0.89,	0.72,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(244	,0.90,	0.73,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(245	,0.91,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(246	,0.91,	0.76,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(247	,0.92,	0.78,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(248	,0.93,	0.80,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(249	,0.93,	0.81,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(250	,0.94,	0.83,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(251	,0.94,	0.84,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(252	,0.95,	0.86,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(253	,0.95,	0.88,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(254	,0.96,	0.89,	1,	1);
		m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ]->SetTableValue(255	,0.96,	0.91,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_SPECTRUM_SIEMENS ];
}


vtkLookupTable * blLookupTables::BuildLutPercepLinRainbow()
{
	if( m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ] == NULL )
	{
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->Build();

		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(0	,0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(1	,0.18,	0,	0.14,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(2	,0.22,	0,	0.18,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(3	,0.24,	0,	0.19,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(4	,0.26,	0,	0.21,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(5	,0.27,	0,	0.23,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(6	,0.28,	0,	0.24,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(7	,0.29,	0,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(8	,0.29,	0,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(9	,0.29,	0,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(10	,0.29,	0,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(11	,0.28,	0,	0.34,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(12	,0.27,	0,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(13	,0.27,	0.01,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(14	,0.26,	0.01,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(15	,0.25,	0.02,	0.4,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(16	,0.24,	0.03,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(17	,0.23,	0.04,	0.43,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(18	,0.22,	0.05,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(19	,0.21,	0.06,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(20	,0.19,	0.07,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(21	,0.18,	0.08,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(22	,0.17,	0.09,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(23	,0.16,	0.11,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(24	,0.16,	0.11,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(25	,0.15,	0.13,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(26	,0.13,	0.14,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(27	,0.11,	0.17,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(28	,0.1,	0.2,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(29	,0.09,	0.22,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(30	,0.09,	0.24,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(31	,0.09,	0.25,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(32	,0.09,	0.25,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(33	,0.09,	0.27,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(34	,0.09,	0.28,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(35	,0.09,	0.28,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(36	,0.09,	0.29,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(37	,0.09,	0.29,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(38	,0.09,	0.29,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(39	,0.09,	0.31,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(40	,0.09,	0.31,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(41	,0.09,	0.31,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(42	,0.09,	0.32,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(43	,0.09,	0.33,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(44	,0.09,	0.33,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(45	,0.09,	0.34,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(46	,0.09,	0.34,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(47	,0.09,	0.35,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(48	,0.09,	0.35,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(49	,0.09,	0.36,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(50	,0.09,	0.36,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(51	,0.09,	0.36,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(52	,0.09,	0.36,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(53	,0.09,	0.38,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(54	,0.09,	0.38,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(55	,0.1,	0.4,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(56	,0.1,	0.4,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(57	,0.1,	0.41,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(58	,0.1,	0.41,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(59	,0.1,	0.41,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(60	,0.1,	0.42,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(61	,0.1,	0.42,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(62	,0.11,	0.44,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(63	,0.11,	0.44,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(64	,0.11,	0.44,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(65	,0.11,	0.45,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(66	,0.11,	0.45,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(67	,0.11,	0.46,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(68	,0.11,	0.46,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(69	,0.11,	0.48,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(70	,0.11,	0.48,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(71	,0.11,	0.48,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(72	,0.11,	0.48,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(73	,0.11,	0.49,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(74	,0.11,	0.49,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(75	,0.12,	0.5,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(76	,0.12,	0.5,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(77	,0.12,	0.51,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(78	,0.12,	0.51,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(79	,0.12,	0.51,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(80	,0.13,	0.53,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(81	,0.13,	0.53,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(82	,0.13,	0.54,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(83	,0.13,	0.54,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(84	,0.13,	0.54,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(85	,0.13,	0.55,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(86	,0.13,	0.55,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(87	,0.14,	0.56,	0.48,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(88	,0.14,	0.56,	0.48,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(89	,0.14,	0.57,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(90	,0.14,	0.57,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(91	,0.14,	0.57,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(92	,0.15,	0.58,	0.46,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(93	,0.15,	0.58,	0.46,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(94	,0.15,	0.59,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(95	,0.15,	0.59,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(96	,0.16,	0.6,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(97	,0.16,	0.6,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(98	,0.16,	0.6,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(99	,0.16,	0.6,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(100	,0.16,	0.61,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(101	,0.16,	0.61,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(102	,0.17,	0.62,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(103	,0.17,	0.62,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(104	,0.17,	0.62,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(105	,0.18,	0.63,	0.41,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(106	,0.18,	0.63,	0.41,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(107	,0.18,	0.64,	0.4,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(108	,0.18,	0.64,	0.4,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(109	,0.19,	0.64,	0.39,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(110	,0.19,	0.64,	0.39,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(111	,0.2,	0.65,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(112	,0.2,	0.65,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(113	,0.2,	0.66,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(114	,0.21,	0.67,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(115	,0.21,	0.67,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(116	,0.21,	0.67,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(117	,0.22,	0.67,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(118	,0.22,	0.67,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(119	,0.22,	0.68,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(120	,0.22,	0.68,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(121	,0.23,	0.69,	0.34,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(122	,0.24,	0.69,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(123	,0.25,	0.7,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(124	,0.25,	0.7,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(125	,0.26,	0.71,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(126	,0.26,	0.71,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(127	,0.27,	0.71,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(128	,0.28,	0.72,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(129	,0.28,	0.72,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(130	,0.28,	0.72,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(131	,0.29,	0.72,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(132	,0.3,	0.73,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(133	,0.31,	0.73,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(134	,0.33,	0.74,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(135	,0.34,	0.75,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(136	,0.36,	0.75,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(137	,0.37,	0.75,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(138	,0.39,	0.76,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(139	,0.4,	0.76,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(140	,0.42,	0.76,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(141	,0.44,	0.77,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(142	,0.44,	0.77,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(143	,0.45,	0.77,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(144	,0.47,	0.77,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(145	,0.48,	0.77,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(146	,0.51,	0.78,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(147	,0.52,	0.78,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(148	,0.54,	0.78,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(149	,0.55,	0.78,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(150	,0.56,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(151	,0.56,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(152	,0.58,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(153	,0.59,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(154	,0.6,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(155	,0.61,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(156	,0.63,	0.78,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(157	,0.65,	0.78,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(158	,0.67,	0.78,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(159	,0.68,	0.78,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(160	,0.68,	0.78,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(161	,0.69,	0.78,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(162	,0.71,	0.78,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(163	,0.72,	0.78,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(164	,0.73,	0.78,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(165	,0.75,	0.78,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(166	,0.77,	0.78,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(167	,0.78,	0.78,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(168	,0.78,	0.78,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(169	,0.8,	0.78,	0.34,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(170	,0.81,	0.77,	0.34,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(171	,0.83,	0.77,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(172	,0.84,	0.77,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(173	,0.85,	0.76,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(174	,0.88,	0.76,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(175	,0.88,	0.76,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(176	,0.9,	0.76,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(177	,0.91,	0.75,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(178	,0.93,	0.75,	0.39,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(179	,0.93,	0.74,	0.41,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(180	,0.94,	0.74,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(181	,0.94,	0.74,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(182	,0.95,	0.73,	0.43,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(183	,0.96,	0.73,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(184	,0.96,	0.72,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(185	,0.97,	0.72,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(186	,0.97,	0.71,	0.48,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(187	,0.97,	0.71,	0.48,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(188	,0.98,	0.71,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(189	,0.98,	0.71,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(190	,0.99,	0.71,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(191	,0.99,	0.71,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(192	,0.99,	0.71,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(193	,1,	0.71,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(194	,1,	0.7,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(195	,1,	0.7,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(196	,1,	0.7,	0.57,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(197	,1,	0.7,	0.57,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(198	,1,	0.7,	0.6,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(199	,1,	0.71,	0.63,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(200	,1,	0.71,	0.64,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(201	,1,	0.71,	0.65,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(202	,1,	0.71,	0.65,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(203	,1,	0.71,	0.66,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(204	,1,	0.71,	0.67,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(205	,1,	0.71,	0.68,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(206	,1,	0.72,	0.69,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(207	,1,	0.72,	0.69,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(208	,1,	0.72,	0.7,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(209	,1,	0.73,	0.7,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(210	,1,	0.73,	0.71,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(211	,1,	0.73,	0.71,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(212	,1,	0.73,	0.71,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(213	,1,	0.73,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(214	,1,	0.74,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(215	,1,	0.74,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(216	,1,	0.74,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(217	,1,	0.75,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(218	,1,	0.75,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(219	,1,	0.75,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(220	,1,	0.76,	0.76,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(221	,1,	0.76,	0.76,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(222	,1,	0.77,	0.77,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(223	,1,	0.78,	0.78,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(224	,1,	0.78,	0.78,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(225	,1,	0.79,	0.79,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(226	,1,	0.79,	0.79,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(227	,1,	0.79,	0.79,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(228	,1,	0.8,	0.8,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(229	,1,	0.8,	0.8,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(230	,1,	0.81,	0.81,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(231	,1,	0.81,	0.81,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(232	,1,	0.82,	0.82,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(233	,1,	0.82,	0.82,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(234	,1,	0.83,	0.83,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(235	,1,	0.84,	0.84,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(236	,1,	0.85,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(237	,1,	0.85,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(238	,1,	0.85,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(239	,1,	0.86,	0.86,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(240	,1,	0.87,	0.87,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(241	,1,	0.87,	0.87,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(242	,1,	0.89,	0.89,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(243	,1,	0.89,	0.89,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(244	,1,	0.9,	0.9,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(245	,1,	0.9,	0.9,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(246	,1,	0.91,	0.91,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(247	,1,	0.92,	0.92,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(248	,1,	0.93,	0.93,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(249	,1,	0.94,	0.94,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(250	,1,	0.95,	0.95,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(251	,1,	0.96,	0.96,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(252	,1,	0.98,	0.98,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(253	,1,	0.98,	0.98,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(254	,1,	0.99,	0.99,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->SetTableValue(255	,1,	1,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ];
}

vtkLookupTable * blLookupTables::BuildLutPercepLinRainbowInverted()
{
	if( m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ] == NULL )
	{
		BuildLutPercepLinRainbow( );

		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ]->Build();
		for ( int i = 0 ; i < 256 ; i++ )
		{
			m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ]->SetTableValue( i,
				m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW ]->GetTableValue( 255 - i ) );
		}
	}

	return m_lutTable[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ];
}

vtkLookupTable * blLookupTables::BuildLutPercepLinHeatedObject()
{
	if ( m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ] == NULL )	
	{
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->Build();

		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(0,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(1,	0.14,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(2,	0.2,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(3,	0.24,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(4,	0.25,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(5,	0.25,	0.01,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(6,	0.27,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(7,	0.27,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(8,	0.28,	0.03,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(9,	0.29,	0.04,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(10,	0.3,	0.05,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(11,	0.31,	0.05,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(12,	0.32,	0.06,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(13,	0.33,	0.07,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(14,	0.33,	0.07,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(15,	0.34,	0.08,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(16,	0.35,	0.09,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(17,	0.36,	0.09,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(18,	0.36,	0.1,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(19,	0.37,	0.1,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(20,	0.37,	0.11,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(21,	0.38,	0.12,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(22,	0.39,	0.12,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(23,	0.4,	0.13,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(24,	0.4,	0.13,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(25,	0.41,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(26,	0.42,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(27,	0.42,	0.15,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(28,	0.43,	0.15,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(29,	0.44,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(30,	0.44,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(31,	0.45,	0.17,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(32,	0.45,	0.17,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(33,	0.46,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(34,	0.47,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(35,	0.47,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(36,	0.47,	0.19,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(37,	0.48,	0.19,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(38,	0.48,	0.2,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(39,	0.49,	0.2,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(40,	0.49,	0.2,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(41,	0.49,	0.21,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(42,	0.5,	0.21,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(43,	0.51,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(44,	0.51,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(45,	0.51,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(46,	0.52,	0.23,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(47,	0.53,	0.23,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(48,	0.53,	0.23,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(49,	0.53,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(50,	0.54,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(51,	0.54,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(52,	0.55,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(53,	0.55,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(54,	0.55,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(55,	0.56,	0.26,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(56,	0.56,	0.26,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(57,	0.56,	0.26,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(58,	0.56,	0.26,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(59,	0.57,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(60,	0.57,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(61,	0.57,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(62,	0.57,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(63,	0.58,	0.28,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(64,	0.58,	0.28,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(65,	0.58,	0.28,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(66,	0.58,	0.28,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(67,	0.59,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(68,	0.59,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(69,	0.6,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(70,	0.6,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(71,	0.6,	0.3,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(72,	0.6,	0.3,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(73,	0.6,	0.3,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(74,	0.61,	0.3,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(75,	0.61,	0.3,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(76,	0.62,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(77,	0.62,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(78,	0.62,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(79,	0.62,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(80,	0.62,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(81,	0.63,	0.32,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(82,	0.63,	0.32,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(83,	0.64,	0.32,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(84,	0.64,	0.32,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(85,	0.64,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(86,	0.64,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(87,	0.65,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(88,	0.65,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(89,	0.65,	0.34,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(90,	0.65,	0.34,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(91,	0.65,	0.34,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(92,	0.66,	0.34,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(93,	0.66,	0.34,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(94,	0.67,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(95,	0.67,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(96,	0.67,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(97,	0.67,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(98,	0.68,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(99,	0.68,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(100,	0.68,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(101,	0.68,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(102,	0.69,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(103,	0.69,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(104,	0.69,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(105,	0.69,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(106,	0.7,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(107,	0.7,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(108,	0.71,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(109,	0.71,	0.39,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(110,	0.71,	0.39,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(111,	0.72,	0.39,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(112,	0.72,	0.39,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(113,	0.73,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(114,	0.73,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(115,	0.73,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(116,	0.73,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(117,	0.74,	0.41,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(118,	0.74,	0.41,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(119,	0.75,	0.41,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(120,	0.75,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(121,	0.75,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(122,	0.76,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(123,	0.76,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(124,	0.76,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(125,	0.77,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(126,	0.77,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(127,	0.77,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(128,	0.77,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(129,	0.78,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(130,	0.78,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(131,	0.78,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(132,	0.79,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(133,	0.79,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(134,	0.8,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(135,	0.8,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(136,	0.8,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(137,	0.81,	0.47,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(138,	0.82,	0.47,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(139,	0.82,	0.47,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(140,	0.82,	0.48,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(141,	0.83,	0.48,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(142,	0.83,	0.48,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(143,	0.84,	0.49,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(144,	0.84,	0.49,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(145,	0.84,	0.49,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(146,	0.85,	0.5,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(147,	0.85,	0.5,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(148,	0.85,	0.5,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(149,	0.86,	0.51,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(150,	0.87,	0.51,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(151,	0.87,	0.51,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(152,	0.87,	0.52,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(153,	0.88,	0.52,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(154,	0.88,	0.52,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(155,	0.88,	0.53,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(156,	0.89,	0.53,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(157,	0.89,	0.53,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(158,	0.89,	0.54,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(159,	0.9,	0.54,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(160,	0.9,	0.54,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(161,	0.91,	0.55,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(162,	0.91,	0.55,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(163,	0.91,	0.55,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(164,	0.92,	0.56,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(165,	0.93,	0.56,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(166,	0.93,	0.56,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(167,	0.93,	0.57,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(168,	0.94,	0.57,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(169,	0.95,	0.58,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(170,	0.95,	0.58,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(171,	0.95,	0.58,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(172,	0.96,	0.59,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(173,	0.96,	0.59,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(174,	0.96,	0.59,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(175,	0.97,	0.6,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(176,	0.97,	0.6,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(177,	0.97,	0.6,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(178,	0.98,	0.61,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(179,	0.98,	0.61,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(180,	0.98,	0.61,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(181,	0.99,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(182,	1,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(183,	1,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(184,	1,	0.63,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(185,	1,	0.63,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(186,	1,	0.64,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(187,	1,	0.64,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(188,	1,	0.64,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(189,	1,	0.65,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(190,	1,	0.65,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(191,	1,	0.65,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(192,	1,	0.66,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(193,	1,	0.66,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(194,	1,	0.66,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(195,	1,	0.67,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(196,	1,	0.67,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(197,	1,	0.68,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(198,	1,	0.68,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(199,	1,	0.68,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(200,	1,	0.69,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(201,	1,	0.69,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(202,	1,	0.7,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(203,	1,	0.7,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(204,	1,	0.71,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(205,	1,	0.71,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(206,	1,	0.71,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(207,	1,	0.72,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(208,	1,	0.72,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(209,	1,	0.73,	0.03,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(210,	1,	0.74,	0.04,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(211,	1,	0.74,	0.05,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(212,	1,	0.75,	0.07,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(213,	1,	0.75,	0.08,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(214,	1,	0.76,	0.1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(215,	1,	0.76,	0.11,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(216,	1,	0.77,	0.14,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(217,	1,	0.78,	0.16,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(218,	1,	0.78,	0.17,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(219,	1,	0.79,	0.2,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(220,	1,	0.8,	0.21,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(221,	1,	0.81,	0.24,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(222,	1,	0.81,	0.25,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(223,	1,	0.82,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(224,	1,	0.83,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(225,	1,	0.84,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(226,	1,	0.85,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(227,	1,	0.86,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(228,	1,	0.87,	0.41,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(229,	1,	0.87,	0.43,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(230,	1,	0.88,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(231,	1,	0.89,	0.48,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(232,	1,	0.91,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(233,	1,	0.92,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(234,	1,	0.93,	0.58,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(235,	1,	0.94,	0.61,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(236,	1,	0.94,	0.63,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(237,	1,	0.95,	0.65,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(238,	1,	0.96,	0.68,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(239,	1,	0.97,	0.71,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(240,	1,	0.98,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(241,	1,	0.99,	0.76,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(242,	1,	0.99,	0.77,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(243,	1,	1,	0.8,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(244,	1,	1,	0.81,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(245,	1,	1,	0.83,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(246,	1,	1,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(247,	1,	1,	0.87,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(248,	1,	1,	0.88,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(249,	1,	1,	0.9,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(250,	1,	1,	0.91,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(251,	1,	1,	0.93,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(252,	1,	1,	0.94,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(253,	1,	1,	0.96,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(254,	1,	1,	0.97,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ]->SetTableValue(255,	1,	1,	1,	1);

	}

	return m_lutTable[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ];
}

vtkLookupTable * blLookupTables::BuildLutPercepLinMagenta()
{
	if ( m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ] == NULL )	
	{
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->Build();

		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(0,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(1,	0.16,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(2,	0.22,	0,	0.02,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(3,	0.24,	0,	0.04,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(4,	0.25,	0,	0.05,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(5,	0.26,	0,	0.05,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(6,	0.27,	0,	0.07,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(7,	0.29,	0,	0.08,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(8,	0.29,	0,	0.09,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(9,	0.31,	0,	0.1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(10,	0.31,	0,	0.11,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(11,	0.33,	0,	0.12,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(12,	0.33,	0,	0.12,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(13,	0.34,	0,	0.13,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(14,	0.35,	0,	0.14,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(15,	0.36,	0,	0.15,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(16,	0.36,	0,	0.15,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(17,	0.37,	0,	0.16,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(18,	0.38,	0,	0.17,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(19,	0.39,	0,	0.18,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(20,	0.4,	0,	0.18,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(21,	0.4,	0,	0.19,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(22,	0.41,	0,	0.2,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(23,	0.42,	0,	0.2,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(24,	0.42,	0,	0.21,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(25,	0.43,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(26,	0.44,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(27,	0.44,	0,	0.22,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(28,	0.44,	0,	0.23,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(29,	0.45,	0,	0.24,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(30,	0.46,	0,	0.24,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(31,	0.47,	0,	0.25,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(32,	0.47,	0,	0.25,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(33,	0.48,	0,	0.26,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(34,	0.49,	0,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(35,	0.49,	0,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(36,	0.5,	0,	0.28,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(37,	0.51,	0,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(38,	0.51,	0,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(39,	0.51,	0,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(40,	0.52,	0,	0.3,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(41,	0.53,	0,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(42,	0.53,	0,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(43,	0.54,	0,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(44,	0.55,	0,	0.32,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(45,	0.55,	0,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(46,	0.56,	0,	0.34,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(47,	0.56,	0,	0.34,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(48,	0.57,	0,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(49,	0.58,	0,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(50,	0.58,	0,	0.36,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(51,	0.59,	0,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(52,	0.59,	0,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(53,	0.6,	0,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(54,	0.6,	0,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(55,	0.61,	0,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(56,	0.62,	0,	0.39,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(57,	0.62,	0,	0.4,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(58,	0.63,	0,	0.4,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(59,	0.64,	0,	0.41,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(60,	0.64,	0,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(61,	0.65,	0,	0.42,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(62,	0.66,	0,	0.43,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(63,	0.67,	0,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(64,	0.67,	0,	0.44,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(65,	0.68,	0,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(66,	0.69,	0,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(67,	0.69,	0,	0.46,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(68,	0.7,	0,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(69,	0.71,	0,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(70,	0.71,	0,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(71,	0.71,	0,	0.48,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(72,	0.72,	0,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(73,	0.73,	0,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(74,	0.73,	0,	0.5,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(75,	0.74,	0,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(76,	0.75,	0,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(77,	0.75,	0,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(78,	0.76,	0,	0.52,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(79,	0.76,	0,	0.53,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(80,	0.77,	0,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(81,	0.78,	0,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(82,	0.78,	0,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(83,	0.79,	0,	0.55,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(84,	0.8,	0,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(85,	0.8,	0,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(86,	0.8,	0,	0.57,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(87,	0.81,	0,	0.57,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(88,	0.82,	0,	0.58,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(89,	0.82,	0,	0.58,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(90,	0.83,	0,	0.59,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(91,	0.84,	0,	0.6,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(92,	0.84,	0,	0.6,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(93,	0.85,	0,	0.61,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(94,	0.86,	0,	0.62,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(95,	0.87,	0,	0.62,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(96,	0.87,	0,	0.63,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(97,	0.87,	0,	0.63,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(98,	0.88,	0,	0.64,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(99,	0.89,	0,	0.64,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(100,	0.89,	0,	0.65,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(101,	0.9,	0,	0.65,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(102,	0.91,	0,	0.66,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(103,	0.91,	0,	0.67,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(104,	0.92,	0,	0.67,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(105,	0.93,	0,	0.68,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(106,	0.93,	0,	0.69,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(107,	0.93,	0,	0.69,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(108,	0.94,	0,	0.69,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(109,	0.95,	0,	0.7,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(110,	0.95,	0,	0.71,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(111,	0.96,	0,	0.71,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(112,	0.96,	0,	0.72,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(113,	0.97,	0.01,	0.72,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(114,	0.98,	0.02,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(115,	0.98,	0.02,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(116,	0.98,	0.02,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(117,	0.98,	0.02,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(118,	0.98,	0.02,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(119,	0.99,	0.04,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(120,	0.99,	0.04,	0.74,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(121,	1,	0.04,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(122,	1,	0.04,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(123,	1,	0.05,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(124,	1,	0.05,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(125,	1,	0.05,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(126,	1,	0.06,	0.76,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(127,	1,	0.07,	0.77,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(128,	1,	0.08,	0.77,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(129,	1,	0.08,	0.77,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(130,	1,	0.09,	0.78,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(131,	1,	0.1,	0.78,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(132,	1,	0.11,	0.79,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(133,	1,	0.12,	0.8,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(134,	1,	0.13,	0.8,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(135,	1,	0.13,	0.81,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(136,	1,	0.15,	0.82,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(137,	1,	0.15,	0.82,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(138,	1,	0.15,	0.82,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(139,	1,	0.16,	0.83,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(140,	1,	0.17,	0.84,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(141,	1,	0.18,	0.84,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(142,	1,	0.19,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(143,	1,	0.2,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(144,	1,	0.21,	0.86,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(145,	1,	0.21,	0.86,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(146,	1,	0.22,	0.87,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(147,	1,	0.22,	0.87,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(148,	1,	0.24,	0.88,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(149,	1,	0.24,	0.89,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(150,	1,	0.25,	0.89,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(151,	1,	0.26,	0.9,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(152,	1,	0.26,	0.9,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(153,	1,	0.27,	0.91,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(154,	1,	0.28,	0.91,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(155,	1,	0.29,	0.92,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(156,	1,	0.3,	0.92,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(157,	1,	0.31,	0.93,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(158,	1,	0.32,	0.94,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(159,	1,	0.32,	0.94,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(160,	1,	0.33,	0.94,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(161,	1,	0.33,	0.95,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(162,	1,	0.35,	0.95,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(163,	1,	0.35,	0.96,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(164,	1,	0.36,	0.97,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(165,	1,	0.37,	0.97,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(166,	1,	0.37,	0.97,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(167,	1,	0.38,	0.98,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(168,	1,	0.39,	0.98,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(169,	1,	0.4,	0.99,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(170,	1,	0.41,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(171,	1,	0.42,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(172,	1,	0.42,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(173,	1,	0.42,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(174,	1,	0.44,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(175,	1,	0.44,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(176,	1,	0.45,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(177,	1,	0.45,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(178,	1,	0.46,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(179,	1,	0.47,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(180,	1,	0.48,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(181,	1,	0.48,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(182,	1,	0.49,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(183,	1,	0.5,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(184,	1,	0.51,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(185,	1,	0.51,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(186,	1,	0.52,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(187,	1,	0.53,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(188,	1,	0.53,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(189,	1,	0.53,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(190,	1,	0.55,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(191,	1,	0.55,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(192,	1,	0.56,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(193,	1,	0.56,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(194,	1,	0.57,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(195,	1,	0.58,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(196,	1,	0.59,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(197,	1,	0.59,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(198,	1,	0.6,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(199,	1,	0.61,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(200,	1,	0.61,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(201,	1,	0.62,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(202,	1,	0.62,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(203,	1,	0.62,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(204,	1,	0.64,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(205,	1,	0.64,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(206,	1,	0.64,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(207,	1,	0.65,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(208,	1,	0.66,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(209,	1,	0.67,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(210,	1,	0.67,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(211,	1,	0.68,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(212,	1,	0.69,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(213,	1,	0.69,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(214,	1,	0.7,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(215,	1,	0.71,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(216,	1,	0.71,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(217,	1,	0.72,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(218,	1,	0.73,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(219,	1,	0.73,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(220,	1,	0.73,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(221,	1,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(222,	1,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(223,	1,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(224,	1,	0.76,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(225,	1,	0.77,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(226,	1,	0.77,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(227,	1,	0.78,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(228,	1,	0.79,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(229,	1,	0.8,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(230,	1,	0.8,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(231,	1,	0.81,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(232,	1,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(233,	1,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(234,	1,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(235,	1,	0.84,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(236,	1,	0.84,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(237,	1,	0.85,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(238,	1,	0.85,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(239,	1,	0.86,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(240,	1,	0.87,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(241,	1,	0.88,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(242,	1,	0.89,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(243,	1,	0.9,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(244,	1,	0.9,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(245,	1,	0.91,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(246,	1,	0.92,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(247,	1,	0.93,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(248,	1,	0.93,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(249,	1,	0.95,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(250,	1,	0.95,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(251,	1,	0.95,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(252,	1,	0.96,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(253,	1,	0.97,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(254,	1,	0.98,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ]->SetTableValue(255,	1,	1,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_PERCEP_LIN_MAGENTA ];
}

vtkLookupTable * blLookupTables::BuildLutOptimal()
{
	if ( m_lutTable[ LUT_TYPE_OPTIMAL ] == NULL )
	{
		m_lutTable[ LUT_TYPE_OPTIMAL ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->Build();

		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(0,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(1,	0.02,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(2,	0.03,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(3,	0.05,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(4,	0.06,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(5,	0.08,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(6,	0.09,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(7,	0.11,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(8,	0.13,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(9,	0.14,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(10,	0.16,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(11,	0.17,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(12,	0.19,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(13,	0.2,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(14,	0.22,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(15,	0.24,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(16,	0.25,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(17,	0.27,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(18,	0.28,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(19,	0.3,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(20,	0.31,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(21,	0.33,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(22,	0.35,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(23,	0.36,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(24,	0.38,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(25,	0.39,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(26,	0.41,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(27,	0.42,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(28,	0.44,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(29,	0.45,	0,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(30,	0.47,	0.01,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(31,	0.47,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(32,	0.47,	0.03,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(33,	0.47,	0.04,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(34,	0.47,	0.05,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(35,	0.47,	0.06,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(36,	0.47,	0.07,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(37,	0.47,	0.08,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(38,	0.47,	0.09,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(39,	0.47,	0.1,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(40,	0.47,	0.11,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(41,	0.47,	0.12,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(42,	0.47,	0.13,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(43,	0.47,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(44,	0.47,	0.15,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(45,	0.47,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(46,	0.47,	0.17,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(47,	0.47,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(48,	0.47,	0.19,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(49,	0.47,	0.2,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(50,	0.47,	0.21,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(51,	0.47,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(52,	0.47,	0.23,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(53,	0.47,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(54,	0.47,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(55,	0.47,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(56,	0.47,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(57,	0.47,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(58,	0.47,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(59,	0.47,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(60,	0.47,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(61,	0.47,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(62,	0.47,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(63,	0.47,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(64,	0.47,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(65,	0.47,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(66,	0.47,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(67,	0.47,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(68,	0.47,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(69,	0.47,	0.39,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(70,	0.47,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(71,	0.47,	0.41,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(72,	0.47,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(73,	0.47,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(74,	0.47,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(75,	0.47,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(76,	0.47,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(77,	0.47,	0.47,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(78,	0.47,	0.48,	0,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(79,	0.47,	0.49,	0.01,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(80,	0.47,	0.5,	0.02,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(81,	0.47,	0.51,	0.03,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(82,	0.47,	0.52,	0.04,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(83,	0.47,	0.53,	0.05,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(84,	0.47,	0.54,	0.06,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(85,	0.47,	0.55,	0.07,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(86,	0.47,	0.56,	0.08,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(87,	0.47,	0.57,	0.09,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(88,	0.47,	0.58,	0.1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(89,	0.48,	0.59,	0.11,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(90,	0.48,	0.6,	0.12,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(91,	0.49,	0.61,	0.13,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(92,	0.49,	0.62,	0.14,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(93,	0.49,	0.63,	0.15,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(94,	0.5,	0.64,	0.16,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(95,	0.5,	0.65,	0.17,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(96,	0.51,	0.66,	0.18,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(97,	0.51,	0.67,	0.19,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(98,	0.51,	0.68,	0.2,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(99,	0.52,	0.69,	0.21,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(100,	0.52,	0.7,	0.22,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(101,	0.52,	0.71,	0.23,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(102,	0.52,	0.72,	0.24,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(103,	0.52,	0.73,	0.25,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(104,	0.52,	0.74,	0.25,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(105,	0.52,	0.75,	0.27,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(106,	0.52,	0.76,	0.27,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(107,	0.52,	0.76,	0.29,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(108,	0.52,	0.78,	0.29,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(109,	0.52,	0.78,	0.31,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(110,	0.52,	0.8,	0.31,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(111,	0.52,	0.8,	0.33,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(112,	0.52,	0.82,	0.33,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(113,	0.52,	0.82,	0.35,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(114,	0.52,	0.84,	0.35,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(115,	0.52,	0.84,	0.36,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(116,	0.52,	0.85,	0.37,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(117,	0.52,	0.86,	0.38,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(118,	0.52,	0.87,	0.39,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(119,	0.52,	0.88,	0.4,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(120,	0.52,	0.89,	0.41,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(121,	0.52,	0.9,	0.42,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(122,	0.52,	0.91,	0.43,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(123,	0.52,	0.92,	0.44,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(124,	0.52,	0.93,	0.45,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(125,	0.52,	0.94,	0.46,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(126,	0.52,	0.95,	0.47,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(127,	0.52,	0.96,	0.48,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(128,	0.52,	0.97,	0.49,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(129,	0.52,	0.98,	0.5,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(130,	0.52,	0.99,	0.51,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(131,	0.52,	1,	0.52,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(132,	0.52,	1,	0.53,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(133,	0.52,	1,	0.54,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(134,	0.52,	1,	0.55,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(135,	0.52,	1,	0.56,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(136,	0.52,	1,	0.57,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(137,	0.52,	1,	0.58,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(138,	0.52,	1,	0.59,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(139,	0.52,	1,	0.6,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(140,	0.52,	1,	0.61,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(141,	0.52,	1,	0.62,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(142,	0.52,	1,	0.63,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(143,	0.52,	1,	0.64,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(144,	0.52,	1,	0.65,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(145,	0.52,	1,	0.66,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(146,	0.52,	1,	0.67,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(147,	0.52,	1,	0.68,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(148,	0.52,	1,	0.69,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(149,	0.52,	1,	0.7,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(150,	0.52,	1,	0.71,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(151,	0.52,	1,	0.72,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(152,	0.52,	1,	0.73,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(153,	0.52,	1,	0.74,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(154,	0.52,	1,	0.75,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(155,	0.52,	1,	0.76,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(156,	0.52,	1,	0.76,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(157,	0.52,	1,	0.78,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(158,	0.52,	1,	0.78,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(159,	0.52,	1,	0.8,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(160,	0.52,	1,	0.8,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(161,	0.52,	1,	0.82,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(162,	0.52,	1,	0.82,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(163,	0.52,	1,	0.84,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(164,	0.52,	1,	0.84,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(165,	0.52,	1,	0.85,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(166,	0.52,	1,	0.86,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(167,	0.52,	1,	0.87,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(168,	0.52,	1,	0.88,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(169,	0.52,	1,	0.89,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(170,	0.52,	1,	0.9,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(171,	0.52,	1,	0.91,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(172,	0.52,	1,	0.92,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(173,	0.52,	1,	0.93,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(174,	0.52,	1,	0.94,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(175,	0.52,	1,	0.95,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(176,	0.52,	1,	0.96,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(177,	0.52,	1,	0.97,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(178,	0.52,	1,	0.98,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(179,	0.52,	1,	0.99,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(180,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(181,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(182,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(183,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(184,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(185,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(186,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(187,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(188,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(189,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(190,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(191,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(192,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(193,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(194,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(195,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(196,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(197,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(198,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(199,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(200,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(201,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(202,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(203,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(204,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(205,	0.53,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(206,	0.54,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(207,	0.55,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(208,	0.56,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(209,	0.57,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(210,	0.58,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(211,	0.59,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(212,	0.6,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(213,	0.61,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(214,	0.62,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(215,	0.63,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(216,	0.64,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(217,	0.65,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(218,	0.66,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(219,	0.67,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(220,	0.68,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(221,	0.69,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(222,	0.7,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(223,	0.71,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(224,	0.72,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(225,	0.73,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(226,	0.74,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(227,	0.75,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(228,	0.76,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(229,	0.76,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(230,	0.78,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(231,	0.78,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(232,	0.8,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(233,	0.8,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(234,	0.82,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(235,	0.82,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(236,	0.84,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(237,	0.84,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(238,	0.85,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(239,	0.86,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(240,	0.87,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(241,	0.88,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(242,	0.89,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(243,	0.9,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(244,	0.91,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(245,	0.92,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(246,	0.93,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(247,	0.94,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(248,	0.95,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(249,	0.96,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(250,	0.97,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(251,	0.98,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(252,	0.99,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(253,	1,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(254,	1,	1,	1,	1);
		m_lutTable[ LUT_TYPE_OPTIMAL ]->SetTableValue(255,	1,	1,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_OPTIMAL ];
}

vtkLookupTable * blLookupTables::BuildLutPercepLinOptimal()
{
	if ( m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ] == NULL )
	{
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->Build();

		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(0,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(1,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(2,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(3,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(4,	0.01,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(5,	0.01,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(6,	0.01,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(7,	0.01,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(8,	0.02,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(9,	0.02,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(10,	0.02,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(11,	0.02,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(12,	0.03,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(13,	0.03,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(14,	0.03,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(15,	0.04,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(16,	0.04,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(17,	0.04,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(18,	0.04,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(19,	0.05,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(20,	0.05,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(21,	0.05,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(22,	0.06,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(23,	0.06,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(24,	0.07,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(25,	0.07,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(26,	0.07,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(27,	0.08,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(28,	0.08,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(29,	0.09,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(30,	0.09,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(31,	0.1,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(32,	0.1,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(33,	0.11,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(34,	0.11,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(35,	0.12,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(36,	0.12,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(37,	0.13,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(38,	0.13,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(39,	0.14,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(40,	0.15,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(41,	0.15,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(42,	0.16,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(43,	0.17,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(44,	0.18,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(45,	0.18,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(46,	0.19,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(47,	0.2,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(48,	0.21,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(49,	0.21,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(50,	0.22,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(51,	0.23,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(52,	0.24,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(53,	0.24,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(54,	0.25,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(55,	0.26,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(56,	0.27,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(57,	0.28,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(58,	0.29,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(59,	0.3,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(60,	0.31,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(61,	0.32,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(62,	0.33,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(63,	0.34,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(64,	0.35,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(65,	0.36,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(66,	0.37,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(67,	0.38,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(68,	0.39,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(69,	0.4,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(70,	0.42,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(71,	0.43,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(72,	0.44,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(73,	0.45,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(74,	0.46,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(75,	0.48,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(76,	0.49,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(77,	0.5,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(78,	0.51,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(79,	0.53,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(80,	0.53,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(81,	0.53,	0,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(82,	0.53,	0.01,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(83,	0.53,	0.01,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(84,	0.53,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(85,	0.53,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(86,	0.53,	0.02,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(87,	0.53,	0.03,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(88,	0.53,	0.04,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(89,	0.53,	0.04,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(90,	0.53,	0.04,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(91,	0.53,	0.05,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(92,	0.53,	0.05,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(93,	0.53,	0.06,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(94,	0.53,	0.07,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(95,	0.53,	0.07,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(96,	0.53,	0.07,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(97,	0.53,	0.08,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(98,	0.53,	0.09,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(99,	0.53,	0.09,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(100,	0.53,	0.1,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(101,	0.53,	0.1,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(102,	0.53,	0.11,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(103,	0.53,	0.11,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(104,	0.53,	0.12,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(105,	0.53,	0.13,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(106,	0.53,	0.13,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(107,	0.53,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(108,	0.53,	0.14,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(109,	0.53,	0.15,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(110,	0.53,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(111,	0.53,	0.16,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(112,	0.53,	0.17,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(113,	0.53,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(114,	0.53,	0.18,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(115,	0.53,	0.19,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(116,	0.53,	0.2,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(117,	0.53,	0.2,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(118,	0.53,	0.21,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(119,	0.53,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(120,	0.53,	0.22,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(121,	0.53,	0.23,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(122,	0.53,	0.24,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(123,	0.53,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(124,	0.53,	0.25,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(125,	0.53,	0.26,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(126,	0.53,	0.27,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(127,	0.53,	0.28,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(128,	0.53,	0.29,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(129,	0.53,	0.3,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(130,	0.53,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(131,	0.53,	0.31,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(132,	0.53,	0.32,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(133,	0.53,	0.33,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(134,	0.53,	0.34,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(135,	0.53,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(136,	0.53,	0.35,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(137,	0.53,	0.36,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(138,	0.53,	0.37,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(139,	0.53,	0.38,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(140,	0.53,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(141,	0.53,	0.4,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(142,	0.53,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(143,	0.53,	0.42,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(144,	0.53,	0.43,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(145,	0.53,	0.44,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(146,	0.53,	0.45,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(147,	0.53,	0.46,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(148,	0.53,	0.47,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(149,	0.53,	0.49,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(150,	0.53,	0.5,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(151,	0.53,	0.51,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(152,	0.53,	0.52,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(153,	0.53,	0.53,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(154,	0.53,	0.54,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(155,	0.53,	0.55,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(156,	0.53,	0.56,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(157,	0.53,	0.58,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(158,	0.53,	0.59,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(159,	0.53,	0.61,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(160,	0.53,	0.62,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(161,	0.53,	0.63,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(162,	0.53,	0.64,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(163,	0.53,	0.65,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(164,	0.53,	0.67,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(165,	0.53,	0.68,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(166,	0.53,	0.69,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(167,	0.53,	0.71,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(168,	0.53,	0.72,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(169,	0.53,	0.74,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(170,	0.53,	0.75,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(171,	0.53,	0.76,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(172,	0.53,	0.78,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(173,	0.53,	0.8,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(174,	0.53,	0.8,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(175,	0.53,	0.82,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(176,	0.53,	0.84,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(177,	0.53,	0.85,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(178,	0.53,	0.87,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(179,	0.53,	0.89,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(180,	0.53,	0.91,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(181,	0.53,	0.93,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(182,	0.53,	0.94,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(183,	0.53,	0.96,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(184,	0.53,	0.98,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(185,	0.53,	1,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(186,	0.53,	1,	0,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(187,	0.53,	1,	0.02,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(188,	0.53,	1,	0.04,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(189,	0.53,	1,	0.06,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(190,	0.53,	1,	0.08,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(191,	0.53,	1,	0.09,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(192,	0.53,	1,	0.11,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(193,	0.53,	1,	0.13,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(194,	0.53,	1,	0.15,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(195,	0.53,	1,	0.17,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(196,	0.53,	1,	0.18,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(197,	0.53,	1,	0.19,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(198,	0.53,	1,	0.21,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(199,	0.53,	1,	0.23,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(200,	0.53,	1,	0.25,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(201,	0.53,	1,	0.27,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(202,	0.53,	1,	0.29,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(203,	0.53,	1,	0.31,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(204,	0.53,	1,	0.33,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(205,	0.53,	1,	0.35,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(206,	0.53,	1,	0.37,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(207,	0.53,	1,	0.38,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(208,	0.53,	1,	0.41,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(209,	0.53,	1,	0.43,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(210,	0.53,	1,	0.45,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(211,	0.53,	1,	0.47,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(212,	0.53,	1,	0.49,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(213,	0.53,	1,	0.51,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(214,	0.53,	1,	0.54,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(215,	0.53,	1,	0.56,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(216,	0.53,	1,	0.58,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(217,	0.53,	1,	0.6,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(218,	0.53,	1,	0.62,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(219,	0.53,	1,	0.65,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(220,	0.53,	1,	0.67,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(221,	0.53,	1,	0.7,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(222,	0.53,	1,	0.73,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(223,	0.53,	1,	0.75,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(224,	0.53,	1,	0.78,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(225,	0.53,	1,	0.8,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(226,	0.53,	1,	0.83,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(227,	0.53,	1,	0.85,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(228,	0.53,	1,	0.88,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(229,	0.53,	1,	0.91,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(230,	0.53,	1,	0.94,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(231,	0.53,	1,	0.97,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(232,	0.53,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(233,	0.53,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(234,	0.55,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(235,	0.57,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(236,	0.6,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(237,	0.61,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(238,	0.63,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(239,	0.66,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(240,	0.67,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(241,	0.69,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(242,	0.71,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(243,	0.74,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(244,	0.75,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(245,	0.78,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(246,	0.8,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(247,	0.82,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(248,	0.84,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(249,	0.86,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(250,	0.88,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(251,	0.91,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(252,	0.93,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(253,	0.94,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(254,	0.97,	1,	1,	1);
		m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ]->SetTableValue(255,	1,	1,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_PERCEP_LIN_OPTIMAL ];
}

vtkLookupTable * blLookupTables::BuildLutBlueToCyan()
{
	if ( m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ] == NULL )
	{
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->Build();

		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(0,	0,	0,	0,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(1,	0,	0,	0.16,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(2,	0,	0.02,	0.22,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(3,	0,	0.04,	0.24,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(4,	0,	0.05,	0.25,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(5,	0,	0.05,	0.26,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(6,	0,	0.07,	0.27,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(7,	0,	0.08,	0.29,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(8,	0,	0.09,	0.29,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(9,	0,	0.1,	0.31,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(10,	0,	0.11,	0.31,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(11,	0,	0.12,	0.33,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(12,	0,	0.12,	0.33,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(13,	0,	0.13,	0.34,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(14,	0,	0.14,	0.35,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(15,	0,	0.15,	0.36,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(16,	0,	0.15,	0.36,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(17,	0,	0.16,	0.37,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(18,	0,	0.17,	0.38,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(19,	0,	0.18,	0.39,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(20,	0,	0.18,	0.4,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(21,	0,	0.19,	0.4,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(22,	0,	0.2,	0.41,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(23,	0,	0.2,	0.42,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(24,	0,	0.21,	0.42,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(25,	0,	0.22,	0.43,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(26,	0,	0.22,	0.44,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(27,	0,	0.22,	0.44,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(28,	0,	0.23,	0.44,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(29,	0,	0.24,	0.45,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(30,	0,	0.24,	0.46,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(31,	0,	0.25,	0.47,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(32,	0,	0.25,	0.47,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(33,	0,	0.26,	0.48,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(34,	0,	0.27,	0.49,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(35,	0,	0.27,	0.49,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(36,	0,	0.28,	0.5,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(37,	0,	0.29,	0.51,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(38,	0,	0.29,	0.51,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(39,	0,	0.29,	0.51,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(40,	0,	0.3,	0.52,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(41,	0,	0.31,	0.53,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(42,	0,	0.31,	0.53,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(43,	0,	0.32,	0.54,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(44,	0,	0.32,	0.55,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(45,	0,	0.33,	0.55,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(46,	0,	0.34,	0.56,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(47,	0,	0.34,	0.56,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(48,	0,	0.35,	0.57,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(49,	0,	0.35,	0.58,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(50,	0,	0.36,	0.58,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(51,	0,	0.37,	0.59,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(52,	0,	0.37,	0.59,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(53,	0,	0.37,	0.6,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(54,	0,	0.38,	0.6,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(55,	0,	0.38,	0.61,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(56,	0,	0.39,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(57,	0,	0.4,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(58,	0,	0.4,	0.63,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(59,	0,	0.41,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(60,	0,	0.42,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(61,	0,	0.42,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(62,	0,	0.43,	0.66,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(63,	0,	0.44,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(64,	0,	0.44,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(65,	0,	0.45,	0.68,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(66,	0,	0.45,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(67,	0,	0.46,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(68,	0,	0.47,	0.7,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(69,	0,	0.47,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(70,	0,	0.47,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(71,	0,	0.48,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(72,	0,	0.49,	0.72,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(73,	0,	0.49,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(74,	0,	0.5,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(75,	0,	0.51,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(76,	0,	0.51,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(77,	0,	0.52,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(78,	0,	0.52,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(79,	0,	0.53,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(80,	0,	0.54,	0.77,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(81,	0,	0.54,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(82,	0,	0.55,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(83,	0,	0.55,	0.79,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(84,	0,	0.56,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(85,	0,	0.56,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(86,	0,	0.57,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(87,	0,	0.57,	0.81,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(88,	0,	0.58,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(89,	0,	0.58,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(90,	0,	0.59,	0.83,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(91,	0,	0.6,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(92,	0,	0.6,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(93,	0,	0.61,	0.85,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(94,	0,	0.62,	0.86,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(95,	0,	0.62,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(96,	0,	0.63,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(97,	0,	0.63,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(98,	0,	0.64,	0.88,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(99,	0,	0.64,	0.89,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(100,	0,	0.65,	0.89,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(101,	0,	0.65,	0.9,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(102,	0,	0.66,	0.91,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(103,	0,	0.67,	0.91,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(104,	0,	0.67,	0.92,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(105,	0,	0.68,	0.93,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(106,	0,	0.69,	0.93,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(107,	0,	0.69,	0.93,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(108,	0,	0.69,	0.94,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(109,	0,	0.7,	0.95,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(110,	0,	0.71,	0.95,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(111,	0,	0.71,	0.96,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(112,	0,	0.72,	0.96,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(113,	0.01,	0.72,	0.97,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(114,	0.02,	0.73,	0.98,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(115,	0.02,	0.73,	0.98,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(116,	0.02,	0.73,	0.98,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(117,	0.02,	0.74,	0.98,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(118,	0.02,	0.74,	0.98,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(119,	0.04,	0.74,	0.99,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(120,	0.04,	0.74,	0.99,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(121,	0.04,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(122,	0.04,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(123,	0.05,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(124,	0.05,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(125,	0.05,	0.75,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(126,	0.06,	0.76,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(127,	0.07,	0.77,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(128,	0.08,	0.77,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(129,	0.08,	0.77,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(130,	0.09,	0.78,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(131,	0.1,	0.78,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(132,	0.11,	0.79,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(133,	0.12,	0.8,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(134,	0.13,	0.8,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(135,	0.13,	0.81,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(136,	0.15,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(137,	0.15,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(138,	0.15,	0.82,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(139,	0.16,	0.83,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(140,	0.17,	0.84,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(141,	0.18,	0.84,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(142,	0.19,	0.85,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(143,	0.2,	0.85,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(144,	0.21,	0.86,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(145,	0.21,	0.86,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(146,	0.22,	0.87,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(147,	0.22,	0.87,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(148,	0.24,	0.88,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(149,	0.24,	0.89,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(150,	0.25,	0.89,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(151,	0.26,	0.9,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(152,	0.26,	0.9,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(153,	0.27,	0.91,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(154,	0.28,	0.91,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(155,	0.29,	0.92,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(156,	0.3,	0.92,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(157,	0.31,	0.93,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(158,	0.32,	0.94,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(159,	0.32,	0.94,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(160,	0.33,	0.94,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(161,	0.33,	0.95,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(162,	0.35,	0.95,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(163,	0.35,	0.96,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(164,	0.36,	0.97,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(165,	0.37,	0.97,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(166,	0.37,	0.97,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(167,	0.38,	0.98,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(168,	0.39,	0.98,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(169,	0.4,	0.99,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(170,	0.41,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(171,	0.42,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(172,	0.42,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(173,	0.42,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(174,	0.44,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(175,	0.44,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(176,	0.45,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(177,	0.45,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(178,	0.46,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(179,	0.47,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(180,	0.48,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(181,	0.48,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(182,	0.49,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(183,	0.5,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(184,	0.51,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(185,	0.51,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(186,	0.52,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(187,	0.53,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(188,	0.53,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(189,	0.53,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(190,	0.55,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(191,	0.55,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(192,	0.56,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(193,	0.56,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(194,	0.57,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(195,	0.58,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(196,	0.59,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(197,	0.59,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(198,	0.6,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(199,	0.61,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(200,	0.61,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(201,	0.62,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(202,	0.62,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(203,	0.62,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(204,	0.64,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(205,	0.64,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(206,	0.64,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(207,	0.65,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(208,	0.66,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(209,	0.67,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(210,	0.67,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(211,	0.68,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(212,	0.69,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(213,	0.69,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(214,	0.7,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(215,	0.71,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(216,	0.71,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(217,	0.72,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(218,	0.73,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(219,	0.73,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(220,	0.73,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(221,	0.75,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(222,	0.75,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(223,	0.75,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(224,	0.76,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(225,	0.77,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(226,	0.77,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(227,	0.78,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(228,	0.79,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(229,	0.8,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(230,	0.8,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(231,	0.81,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(232,	0.82,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(233,	0.82,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(234,	0.82,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(235,	0.84,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(236,	0.84,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(237,	0.85,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(238,	0.85,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(239,	0.86,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(240,	0.87,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(241,	0.88,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(242,	0.89,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(243,	0.9,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(244,	0.9,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(245,	0.91,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(246,	0.92,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(247,	0.93,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(248,	0.93,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(249,	0.95,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(250,	0.95,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(251,	0.95,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(252,	0.96,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(253,	0.97,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(254,	0.98,	1,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ]->SetTableValue(255,	1,	1,	1,	1);
	}

	return m_lutTable[ LUT_TYPE_BLUE_TO_CYAN ];
}

vtkLookupTable * blLookupTables::BuildLutBlueToYellow()
{
	if ( m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ] == NULL )
	{
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->Build();

		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(0,	0.03,	0.03,	1,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(1,	0.09,	0.09,	0.99,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(2,	0.12,	0.12,	0.98,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(3,	0.14,	0.14,	0.97,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(4,	0.16,	0.16,	0.97,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(5,	0.17,	0.17,	0.96,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(6,	0.18,	0.18,	0.95,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(7,	0.2,	0.2,	0.95,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(8,	0.2,	0.2,	0.94,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(9,	0.22,	0.22,	0.94,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(10,	0.22,	0.22,	0.93,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(11,	0.23,	0.23,	0.93,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(12,	0.24,	0.24,	0.92,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(13,	0.25,	0.25,	0.92,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(14,	0.25,	0.25,	0.91,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(15,	0.26,	0.26,	0.91,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(16,	0.27,	0.27,	0.9,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(17,	0.27,	0.27,	0.9,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(18,	0.28,	0.28,	0.89,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(19,	0.28,	0.28,	0.89,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(20,	0.29,	0.29,	0.89,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(21,	0.29,	0.29,	0.88,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(22,	0.3,	0.3,	0.88,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(23,	0.31,	0.31,	0.88,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(24,	0.31,	0.31,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(25,	0.31,	0.31,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(26,	0.32,	0.32,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(27,	0.32,	0.32,	0.87,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(28,	0.33,	0.33,	0.86,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(29,	0.33,	0.33,	0.86,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(30,	0.34,	0.34,	0.85,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(31,	0.34,	0.34,	0.85,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(32,	0.35,	0.35,	0.85,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(33,	0.35,	0.35,	0.85,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(34,	0.35,	0.35,	0.85,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(35,	0.36,	0.36,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(36,	0.36,	0.36,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(37,	0.36,	0.36,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(38,	0.37,	0.37,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(39,	0.37,	0.37,	0.84,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(40,	0.38,	0.38,	0.83,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(41,	0.38,	0.38,	0.83,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(42,	0.38,	0.38,	0.83,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(43,	0.38,	0.38,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(44,	0.39,	0.39,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(45,	0.39,	0.39,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(46,	0.4,	0.4,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(47,	0.4,	0.4,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(48,	0.4,	0.4,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(49,	0.41,	0.41,	0.82,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(50,	0.41,	0.41,	0.81,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(51,	0.41,	0.41,	0.81,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(52,	0.42,	0.42,	0.81,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(53,	0.42,	0.42,	0.81,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(54,	0.42,	0.42,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(55,	0.43,	0.43,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(56,	0.43,	0.43,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(57,	0.43,	0.43,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(58,	0.44,	0.44,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(59,	0.44,	0.44,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(60,	0.44,	0.44,	0.8,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(61,	0.45,	0.45,	0.79,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(62,	0.45,	0.45,	0.79,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(63,	0.45,	0.45,	0.79,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(64,	0.45,	0.45,	0.79,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(65,	0.46,	0.46,	0.79,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(66,	0.46,	0.46,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(67,	0.46,	0.46,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(68,	0.47,	0.47,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(69,	0.47,	0.47,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(70,	0.47,	0.47,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(71,	0.47,	0.47,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(72,	0.48,	0.48,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(73,	0.48,	0.48,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(74,	0.49,	0.49,	0.78,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(75,	0.49,	0.49,	0.77,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(76,	0.49,	0.49,	0.77,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(77,	0.49,	0.49,	0.77,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(78,	0.5,	0.5,	0.77,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(79,	0.5,	0.5,	0.77,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(80,	0.5,	0.5,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(81,	0.51,	0.51,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(82,	0.51,	0.51,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(83,	0.51,	0.51,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(84,	0.51,	0.51,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(85,	0.52,	0.52,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(86,	0.52,	0.52,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(87,	0.52,	0.52,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(88,	0.53,	0.53,	0.76,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(89,	0.53,	0.53,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(90,	0.53,	0.53,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(91,	0.53,	0.53,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(92,	0.54,	0.54,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(93,	0.54,	0.54,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(94,	0.55,	0.55,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(95,	0.55,	0.55,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(96,	0.55,	0.55,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(97,	0.55,	0.55,	0.75,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(98,	0.56,	0.56,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(99,	0.56,	0.56,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(100,	0.56,	0.56,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(101,	0.56,	0.56,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(102,	0.56,	0.56,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(103,	0.57,	0.57,	0.74,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(104,	0.57,	0.57,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(105,	0.58,	0.58,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(106,	0.58,	0.58,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(107,	0.58,	0.58,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(108,	0.58,	0.58,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(109,	0.58,	0.58,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(110,	0.59,	0.59,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(111,	0.59,	0.59,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(112,	0.6,	0.6,	0.73,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(113,	0.6,	0.6,	0.72,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(114,	0.6,	0.6,	0.72,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(115,	0.6,	0.6,	0.72,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(116,	0.6,	0.6,	0.72,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(117,	0.61,	0.61,	0.72,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(118,	0.61,	0.61,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(119,	0.62,	0.62,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(120,	0.62,	0.62,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(121,	0.62,	0.62,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(122,	0.62,	0.62,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(123,	0.62,	0.62,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(124,	0.63,	0.63,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(125,	0.63,	0.63,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(126,	0.64,	0.64,	0.71,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(127,	0.64,	0.64,	0.7,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(128,	0.64,	0.64,	0.7,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(129,	0.64,	0.64,	0.7,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(130,	0.64,	0.64,	0.7,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(131,	0.65,	0.65,	0.7,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(132,	0.65,	0.65,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(133,	0.65,	0.65,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(134,	0.65,	0.65,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(135,	0.66,	0.66,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(136,	0.66,	0.66,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(137,	0.66,	0.66,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(138,	0.67,	0.67,	0.69,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(139,	0.67,	0.67,	0.68,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(140,	0.67,	0.67,	0.68,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(141,	0.67,	0.67,	0.68,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(142,	0.68,	0.68,	0.68,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(143,	0.68,	0.68,	0.68,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(144,	0.68,	0.68,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(145,	0.69,	0.69,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(146,	0.69,	0.69,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(147,	0.69,	0.69,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(148,	0.69,	0.69,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(149,	0.7,	0.7,	0.67,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(150,	0.7,	0.7,	0.66,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(151,	0.7,	0.7,	0.66,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(152,	0.71,	0.71,	0.66,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(153,	0.71,	0.71,	0.66,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(154,	0.71,	0.71,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(155,	0.71,	0.71,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(156,	0.72,	0.72,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(157,	0.72,	0.72,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(158,	0.72,	0.72,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(159,	0.73,	0.73,	0.65,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(160,	0.73,	0.73,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(161,	0.73,	0.73,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(162,	0.73,	0.73,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(163,	0.74,	0.74,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(164,	0.74,	0.74,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(165,	0.74,	0.74,	0.64,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(166,	0.75,	0.75,	0.63,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(167,	0.75,	0.75,	0.63,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(168,	0.75,	0.75,	0.63,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(169,	0.75,	0.75,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(170,	0.76,	0.76,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(171,	0.76,	0.76,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(172,	0.76,	0.76,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(173,	0.76,	0.76,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(174,	0.77,	0.77,	0.62,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(175,	0.77,	0.77,	0.61,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(176,	0.77,	0.77,	0.61,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(177,	0.78,	0.78,	0.61,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(178,	0.78,	0.78,	0.6,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(179,	0.78,	0.78,	0.6,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(180,	0.78,	0.78,	0.6,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(181,	0.79,	0.79,	0.6,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(182,	0.79,	0.79,	0.59,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(183,	0.79,	0.79,	0.59,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(184,	0.8,	0.8,	0.59,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(185,	0.8,	0.8,	0.58,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(186,	0.8,	0.8,	0.58,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(187,	0.8,	0.8,	0.58,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(188,	0.81,	0.81,	0.58,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(189,	0.81,	0.81,	0.57,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(190,	0.81,	0.81,	0.57,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(191,	0.82,	0.82,	0.57,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(192,	0.82,	0.82,	0.56,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(193,	0.82,	0.82,	0.56,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(194,	0.82,	0.82,	0.56,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(195,	0.83,	0.83,	0.56,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(196,	0.83,	0.83,	0.55,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(197,	0.83,	0.83,	0.55,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(198,	0.84,	0.84,	0.55,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(199,	0.84,	0.84,	0.54,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(200,	0.84,	0.84,	0.54,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(201,	0.84,	0.84,	0.54,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(202,	0.85,	0.85,	0.53,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(203,	0.85,	0.85,	0.53,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(204,	0.85,	0.85,	0.53,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(205,	0.85,	0.85,	0.52,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(206,	0.86,	0.86,	0.52,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(207,	0.86,	0.86,	0.51,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(208,	0.86,	0.86,	0.51,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(209,	0.87,	0.87,	0.51,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(210,	0.87,	0.87,	0.5,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(211,	0.87,	0.87,	0.5,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(212,	0.87,	0.87,	0.49,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(213,	0.88,	0.88,	0.49,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(214,	0.88,	0.88,	0.49,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(215,	0.88,	0.88,	0.48,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(216,	0.89,	0.89,	0.48,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(217,	0.89,	0.89,	0.47,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(218,	0.89,	0.89,	0.47,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(219,	0.89,	0.89,	0.46,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(220,	0.9,	0.9,	0.46,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(221,	0.9,	0.9,	0.45,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(222,	0.9,	0.9,	0.45,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(223,	0.91,	0.91,	0.44,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(224,	0.91,	0.91,	0.44,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(225,	0.91,	0.91,	0.43,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(226,	0.91,	0.91,	0.43,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(227,	0.92,	0.92,	0.42,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(228,	0.92,	0.92,	0.42,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(229,	0.92,	0.92,	0.41,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(230,	0.93,	0.93,	0.4,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(231,	0.93,	0.93,	0.4,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(232,	0.93,	0.93,	0.39,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(233,	0.93,	0.93,	0.38,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(234,	0.94,	0.94,	0.38,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(235,	0.94,	0.94,	0.37,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(236,	0.94,	0.94,	0.36,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(237,	0.95,	0.95,	0.36,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(238,	0.95,	0.95,	0.35,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(239,	0.95,	0.95,	0.34,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(240,	0.95,	0.95,	0.33,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(241,	0.96,	0.96,	0.32,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(242,	0.96,	0.96,	0.31,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(243,	0.96,	0.96,	0.3,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(244,	0.96,	0.96,	0.29,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(245,	0.97,	0.97,	0.28,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(246,	0.97,	0.97,	0.27,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(247,	0.97,	0.97,	0.25,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(248,	0.98,	0.98,	0.24,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(249,	0.98,	0.98,	0.23,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(250,	0.98,	0.98,	0.21,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(251,	0.98,	0.98,	0.19,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(252,	0.99,	0.99,	0.17,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(253,	0.99,	0.99,	0.15,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(254,	0.99,	0.99,	0.11,	1);
		m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ]->SetTableValue(255,	1,	1,	0.05,	1);
	}

	return m_lutTable[ LUT_TYPE_BLUE_TO_YELLOW ];
}

vtkLookupTable * blLookupTables::BuildLutWhiteToRed()
{
	if ( m_lutTable[ LUT_TYPE_WHITE_TO_RED ] == NULL )
	{
		m_lutTable[ LUT_TYPE_WHITE_TO_RED ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_WHITE_TO_RED ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_WHITE_TO_RED ]->Build();

		for (int i=0; i<256; i++) {
			m_lutTable[ LUT_TYPE_WHITE_TO_RED ]->SetTableValue(
				i, 
				1.0, 
				(255-(double)i)/255.0, 
				(255-(double)i)/255.0, 
				1.0);
		}
	}

	return m_lutTable[ LUT_TYPE_WHITE_TO_RED ];
}


vtkLookupTable * blLookupTables::BuildLookupTableBLACKWHITE()
{
	if ( m_lutTable[ LUT_TYPE_BLACK_WHITE ] == NULL )
	{

		m_lutTable[ LUT_TYPE_BLACK_WHITE ] = vtkSmartPointer<vtkLookupTable>::New();
		
		m_lutTable[ LUT_TYPE_BLACK_WHITE ]->SetNumberOfTableValues(256);
		
		double floatValue;
		int i;
		for (i = 0; i < 256; i++)
		{
			floatValue = (double) i / 256.0;

			m_lutTable[ LUT_TYPE_BLACK_WHITE ]->SetTableValue(i, floatValue, floatValue, floatValue);
		}
	}

	return m_lutTable[ LUT_TYPE_BLACK_WHITE ];
}



vtkLookupTable * blLookupTables::BuildLookupTableWHITEBLACK()
{
	if ( m_lutTable[ LUT_TYPE_WHITE_BLACK ] == NULL )
	{

		m_lutTable[ LUT_TYPE_WHITE_BLACK ] = vtkSmartPointer<vtkLookupTable>::New();
		
		m_lutTable[ LUT_TYPE_WHITE_BLACK ]->SetNumberOfTableValues(256);
		
		double floatValue;
		int i;
		for (i = 0; i < 256; i++)
		{
			floatValue = (double) (256 - i) / 256.0;

			m_lutTable[ LUT_TYPE_WHITE_BLACK ]->SetTableValue(
				i, floatValue, floatValue, floatValue);
		}
	}

	return m_lutTable[ LUT_TYPE_WHITE_BLACK ];
}


vtkLookupTable * blLookupTables::BuildLookupTableHOTMETAL()
{
	if ( m_lutTable[ LUT_TYPE_HOT_METAL ] == NULL )
	{
		m_lutTable[ LUT_TYPE_HOT_METAL ] = vtkSmartPointer<vtkLookupTable>::New();

		const unsigned int NUM_COLORS = 256;

		// generates a 'hot metal' style color lookup table
		// ----------------------------------	
		m_lutTable[ LUT_TYPE_HOT_METAL ]->SetNumberOfTableValues(NUM_COLORS);
		m_lutTable[ LUT_TYPE_HOT_METAL ]->SetRampToLinear();

		for (size_t i = 0; i < NUM_COLORS; i++)
		{		
			int intValue255Scale;
			int red, green, blue;

			// int value in 255-value scale
			intValue255Scale = (int) (((double) i / 255.0) * 255.0);

			// red
			red = (intValue255Scale - 52) * 3;
			if (red < 0) red = 0;
			else if (red > 255) red = 255;

			// green
			green = (intValue255Scale - 108) * 2;
			if (green < 0) green = 0;
			else if (green > 255) green = 255;

			// blue
			if (intValue255Scale >= 0 && intValue255Scale <= 55)
				blue = intValue255Scale * 4;

			else if (intValue255Scale >= 56 && intValue255Scale <= 118)
				blue = 220 - ((intValue255Scale - 55) * 3);

			else if (intValue255Scale >= 119 && intValue255Scale <= 235)
				blue = 0;
			else
				blue = (intValue255Scale - 235) * 10;

			if (blue < 0) blue = 0;
			else if (blue > 255) blue = 255;

			// conversion to float		
			double redFloat = (double) red / 255.0;
			double greenFloat = (double) green / 255.0;
			double blueFloat = (double) blue / 255.0;

			m_lutTable[ LUT_TYPE_HOT_METAL ]->SetTableValue(i, redFloat, greenFloat, blueFloat);
		}

	}

	return m_lutTable[ LUT_TYPE_HOT_METAL ];
}


vtkLookupTable * blLookupTables::BuildLookupTableICE()
{

	if ( m_lutTable[ LUT_TYPE_ICE ] == NULL )
	{
		m_lutTable[ LUT_TYPE_ICE ] = vtkSmartPointer<vtkLookupTable>::New();

		const unsigned int NUM_COLORS = 32;

		// generates a 'hot metal' style color lookup table
		// ----------------------------------	
		m_lutTable[ LUT_TYPE_ICE ]->SetNumberOfTableValues(NUM_COLORS);
		m_lutTable[ LUT_TYPE_ICE ]->SetRampToLinear();

		unsigned char red[NUM_COLORS] = {0,0,0,0,0,0,19,29,50,48,79,112,134,158,186,201,217,229,242,250,250,250,250,251,250,250,250,250,251,251,243,230};
		unsigned char green[NUM_COLORS] = {156,165,176,184,190,196,193,184,171,162,146,125,107,93,81,87,92,97,95,93,93,90,85,69,64,54,47,35,19,0,4,0};
		unsigned char blue[NUM_COLORS] = {140,147,158,166,170,176,209,220,234,225,236,246,250,251,250,250,245,230,230,222,202,180,163,142,123,114,106,94,84,64,26,27};

		for (size_t i = 0; i < NUM_COLORS; i++)
		{		

			// conversion to float		
			double redFloat = (double) red[i] / 255.0;
			double greenFloat = (double) green[i] / 255.0;
			double blueFloat = (double) blue[i] / 255.0;

			m_lutTable[ LUT_TYPE_ICE ]->SetTableValue(i, redFloat, greenFloat, blueFloat);
		}

	}


	return m_lutTable[ LUT_TYPE_ICE ];
}


vtkLookupTable * blLookupTables::BuildLookupTableHOT()
{
	if ( m_lutTable[ LUT_TYPE_HOT ] == NULL )
	{
		m_lutTable[ LUT_TYPE_HOT ] = vtkSmartPointer<vtkLookupTable>::New();

		const unsigned int NUM_COLORS = 256;

		// generates a 'hot' style color lookup table
		// ----------------------------------	
		m_lutTable[ LUT_TYPE_HOT ]->SetNumberOfTableValues(256);
		m_lutTable[ LUT_TYPE_HOT ]->SetRampToLinear();

		unsigned char red[NUM_COLORS] = {1,3,5,8,11,13,16,19,21,24,27,29,32,35,37,40,43,45,48,50,53,56,58,61,64,66,69,72,74,77,80,82,85,88,90,93,96,98,101,104,106,109,112,114,117,120,122,125,128,130,133,135,138,141,143,146,149,151,154,157,159,162,165,167,170,173,175,178,181,183,186,189,191,194,197,199,202,205,207,210,212,215,218,220,223,226,228,231,234,236,239,242,244,247,250,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

		unsigned char green[NUM_COLORS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,5,8,11,13,16,19,21,24,27,29,32,35,37,40,43,45,48,50,53,56,58,61,64,66,69,72,74,77,80,82,85,88,90,93,96,98,101,104,106,109,112,114,117,120,122,125,128,130,133,135,138,141,143,146,149,151,154,157,159,162,165,167,170,173,175,178,181,183,186,189,191,194,197,199,202,205,207,210,212,215,218,220,223,226,228,231,234,236,239,242,244,247,250,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

		unsigned char blue[NUM_COLORS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,131,135,139,143,147,151,155,159,163,167,171,175,179,183,187,191,195,199,203,207,211,215,219,223,227,231,235,239,243,247,251,255};

		for (size_t i = 0; i < NUM_COLORS; i++)
		{		

			double redFloat = (double) red[i]/255.0;
			double greenFloat = (double) green[i]/255.0;
			double blueFloat = (double) blue[i]/255.0;

			m_lutTable[ LUT_TYPE_HOT ]->SetTableValue(i, redFloat, greenFloat, blueFloat);
		}
	}

	return m_lutTable[ LUT_TYPE_HOT ];

}

vtkLookupTable * blLookupTables::BuildLookupTableGE()
{
	if ( m_lutTable[ LUT_TYPE_GE ] == NULL )
	{
		m_lutTable[ LUT_TYPE_GE ] = vtkSmartPointer<vtkLookupTable>::New();

		const unsigned int NUM_COLORS = 256;

		// generates a 'GE' style color lookup table
		// ----------------------------------	
		m_lutTable[ LUT_TYPE_GE ]->SetNumberOfTableValues(NUM_COLORS);
		m_lutTable[ LUT_TYPE_GE ]->SetRampToLinear();

		unsigned char red[NUM_COLORS] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59,61,63,64,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,127,129,131,133,135,137,139,141,143,145,147,149,151,153,155,157,159,161,163,165,167,169,171,173,175,177,179,181,183,185,187,189,191,191,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238,240,242,244,246,248,250,252,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,255};
		unsigned char green[NUM_COLORS] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,6,8,10,12,14,16,18,20,23,25,27,29,31,33,35,37,39,42,44,46,48,50,52,54,56,58,60,63,64,65,67,69,71,73,75,77,80,82,84,86,88,90,92,94,96,98,101,103,105,107,109,111,113,115,117,120,122,124,126,128,130,128,126,124,122,120,118,116,114,112,110,108,106,104,102,100,98,96,94,92,90,88,86,84,82,80,78,76,74,72,70,68,66,64,64,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,4,6,9,11,13,16,18,20,23,25,28,30,32,35,37,39,42,44,47,49,51,54,56,58,61,63,64,66,68,71,73,75,78,80,82,85,87,90,92,94,97,99,101,104,106,109,111,113,116,118,120,123,125,127,130,132,135,137,139,142,144,146,149,151,154,156,158,161,163,165,168,170,173,175,177,180,182,184,187,189,191,192,194,197,199,201,204,206,208,211,213,216,218,220,223,225,227,230,232,235,237,239,242,244,246,249,251,253,253,253,253,253,255};
		unsigned char blue[NUM_COLORS] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,129,131,133,135,137,139,141,143,145,147,149,151,153,155,157,159,161,163,165,167,169,171,173,175,177,179,181,183,185,187,189,191,191,193,195,197,199,201,203,205,207,209,211,213,215,217,219,221,223,225,227,229,231,233,235,237,239,241,243,245,247,249,251,253,250,246,242,238,234,230,226,222,218,214,210,206,202,198,194,191,188,184,180,176,172,168,164,160,156,152,148,144,140,136,132,128,124,120,116,112,108,104,100,96,92,88,84,80,76,72,68,64,62,58,54,50,46,42,38,34,30,26,22,18,14,10,6,2,7,13,19,25,31,36,42,48,54,60,64,69,75,81,87,93,98,104,110,116,122,127,133,139,145,151,157,162,168,174,180,186,191,195,201,207,213,219,224,230,236,242,248,253,253,253,253,253,255};

		for (size_t i = 0; i < NUM_COLORS; i++)
		{		

			double redFloat = (double) red[i]/255.0;
			double greenFloat = (double) green[i]/255.0;
			double blueFloat = (double) blue[i]/255.0;

			m_lutTable[ LUT_TYPE_GE ]->SetTableValue(i, redFloat, greenFloat, blueFloat);
		}
	}

	return m_lutTable[ LUT_TYPE_GE ];

}


/** 
 *	Builds a lookup table with a primary (red, green, blue).
 */
//--------------------------------------------------------------------------
vtkLookupTable * blLookupTables::BuildLookupTablePRIMARYCOLOR(COLOR_TYPE color)
//--------------------------------------------------------------------------
{
	if ( m_lutTable[ LUT_TYPE_PRIMARY_COLOR ] == NULL )
	{
		m_lutTable[ LUT_TYPE_PRIMARY_COLOR ] = vtkSmartPointer<vtkLookupTable>::New();

		const unsigned int NUM_COLORS = 256;

		m_lutTable[ LUT_TYPE_PRIMARY_COLOR ]->SetNumberOfTableValues(NUM_COLORS);
		m_lutTable[ LUT_TYPE_PRIMARY_COLOR ]->SetRampToLinear();

		// conversion to float		
		double floatValue;

		for (size_t i = 0; i < NUM_COLORS; i++)
		{		
			// conversion to float		
			floatValue = (double) i / 255.0;
			
			switch(color )
			{
			case COLOR_RED:m_lutTable[ LUT_TYPE_PRIMARY_COLOR ]->SetTableValue(i, floatValue, 0, 0);break;
			case COLOR_GREEN:m_lutTable[ LUT_TYPE_PRIMARY_COLOR ]->SetTableValue(i, 0, floatValue, 0);break;
			case COLOR_BLUE:m_lutTable[ LUT_TYPE_PRIMARY_COLOR ]->SetTableValue(i, 0, 0, floatValue);break;
			}
		}
	}

	return m_lutTable[ LUT_TYPE_PRIMARY_COLOR ];
}

vtkLookupTable * blLookupTables::BuildLookupTable16Colors()
{
	if ( m_lutTable[ LUT_TYPE_16_COLORS ] == NULL )
	{
		m_lutTable[ LUT_TYPE_16_COLORS ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetNumberOfTableValues( 16 );
		m_lutTable[ LUT_TYPE_16_COLORS ]->Build();

		// RED
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(0,	1,	0,	0,	1);
		// BLUE
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(1,	0,	0,	1,	1);
		// AQUA = 100% GREEN + 100% BLUE
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(2,	0,	1,	1,	1);
		// GREEN
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(3,	0,	1,	0,	1);
		// YELLOW = 100% RED + 100% GREEN
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(4,	1,	1,	0,	1);
		// MAGENTA = 100% RED + 100% BLUE
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(5,	1,	0,	1,	1);
		// MAROON = 50% RED
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(6,	0.5,	0,	0,	1);
		// DARK BLUE
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(7,	0,	0,	0.5,	1);
		// DARK AQUA
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(8,	0,	0.5,	0.5,	1);
		// DARK GREEN
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(9,	0,	0.5,	0,	1);
		// OLIVE = 50% RED + 50% GREEN
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(10,	0.5,	0.5,	0,	1);
		// PURPLE = 50% RED + 50% BLUE
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(11,	0.5,	0,	0.5,	1);
		// 25% RED
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(12,	0.25,	0,	0,	1);
		// 25% BLUE
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(13,	0,	0,	0.25,	1);
		// 25% AQUA
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(14,	0,	0.25,	0.25,	1);
		// 25% GREEN
		m_lutTable[ LUT_TYPE_16_COLORS ]->SetTableValue(15,	0,	0.25,	0,	1);
	}

	return m_lutTable[ LUT_TYPE_16_COLORS ];
}


vtkLookupTable * blLookupTables::BuildLookupTableRaibowHue()
{
	if ( m_lutTable[ LUT_TYPE_RAINBOW_HUE ] == NULL )
	{
		m_lutTable[ LUT_TYPE_RAINBOW_HUE ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_RAINBOW_HUE ]->SetNumberOfTableValues( 256 );
		m_lutTable[ LUT_TYPE_RAINBOW_HUE ]->SetHueRange( 0.0, 0.667 );
		m_lutTable[ LUT_TYPE_RAINBOW_HUE ]->Build();
	}

	return m_lutTable[ LUT_TYPE_RAINBOW_HUE ];
}

vtkLookupTable * blLookupTables::BuildLookupTableRaibowHueInverted()
{
	if ( m_lutTable[ LUT_TYPE_RAINBOW_HUE_INVERTED ] == NULL )
	{
		m_lutTable[ LUT_TYPE_RAINBOW_HUE_INVERTED ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_RAINBOW_HUE_INVERTED ]->SetNumberOfTableValues( 256 );
		m_lutTable[ LUT_TYPE_RAINBOW_HUE_INVERTED ]->SetHueRange( 0.667, 0.0 );
		m_lutTable[ LUT_TYPE_RAINBOW_HUE_INVERTED ]->Build();
	}

	return m_lutTable[ LUT_TYPE_RAINBOW_HUE_INVERTED ];
}


vtkLookupTable * blLookupTables::BuildLutMITKGray()
{
	if( m_lutTable[ LUT_TYPE_MITK_GRAY ] == NULL )
	{
		m_lutTable[ LUT_TYPE_MITK_GRAY ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_MITK_GRAY ]->SetTableRange( -1024.0, 4096.0 );
		m_lutTable[ LUT_TYPE_MITK_GRAY ]->SetSaturationRange( 0.0, 0.0 );
		m_lutTable[ LUT_TYPE_MITK_GRAY ]->SetHueRange( 0.0, 0.0 );
		m_lutTable[ LUT_TYPE_MITK_GRAY ]->SetValueRange( 0.0, 1.0 );
		m_lutTable[ LUT_TYPE_MITK_GRAY ]->Build();
		m_lutTable[ LUT_TYPE_MITK_GRAY ]->SetTableValue( 0, 0.0, 0.0, 0.0, 0.0 );

	}

	return m_lutTable[ LUT_TYPE_MITK_GRAY ];
}

std::string blLookupTables::GetLUTName( LUT_TYPE type )
{
	BuildLutNames( );
	return m_LutName[ type ];
}

int blLookupTables::GetNumberOfLuts()
{
	BuildLutNames( );
	return m_LutName.size();
}

void blLookupTables::BuildLutNames()
{
	if ( m_LutName.empty() )
	{
		m_LutName[ LUT_TYPE_NON_PERCEP_LIN_GRAY ] = "Non Percep. Lin. gray";
		m_LutName[ LUT_TYPE_PERCEP_LIN_GRAY ] = "Percep. Lin. gray";
		m_LutName[ LUT_TYPE_PERCEP_LIN_RAINBOW ] = "Lin rainbow";
		m_LutName[ LUT_TYPE_PERCEP_LIN_RAINBOW_INVERTED ] = "Lin rainbow inverted";
		m_LutName[ LUT_TYPE_PERCEP_LIN_HEATED_OBJECT ] = "Lin heated object";
		m_LutName[ LUT_TYPE_PERCEP_LIN_MAGENTA ] = "Lin magenta";
		m_LutName[ LUT_TYPE_OPTIMAL ] = "Optimal";
		m_LutName[ LUT_TYPE_PERCEP_LIN_OPTIMAL ] = "Percep. lin. optimal";
		m_LutName[ LUT_TYPE_BLUE_TO_CYAN ] = "Blue to cyan";
		m_LutName[ LUT_TYPE_BLUE_TO_YELLOW ] = "Blue to yellow";
		m_LutName[ LUT_TYPE_WHITE_TO_RED ] = "White to red";
		m_LutName[ LUT_TYPE_BLACK_WHITE ] = "Black to white";
		m_LutName[ LUT_TYPE_WHITE_BLACK ] = "White to black";
		m_LutName[ LUT_TYPE_HOT_METAL ] = "Hot metal";
		m_LutName[ LUT_TYPE_ICE ] = "Ice";
		m_LutName[ LUT_TYPE_HOT ] = "Hot";
		m_LutName[ LUT_TYPE_GE ] = "Ge";
		m_LutName[ LUT_TYPE_PRIMARY_COLOR ] = "Primary color";
		m_LutName[ LUT_TYPE_16_COLORS ] = "16 colors";
		m_LutName[ LUT_TYPE_RAINBOW_HUE ] = "Rainbow hue";
		m_LutName[ LUT_TYPE_RAINBOW_HUE_INVERTED ] = "Rainbow hue inverted";
		m_LutName[ LUT_TYPE_RAINBOW_GE ] = "Rainbow GE";
		m_LutName[ LUT_TYPE_SPECTRUM_SIEMENS ] = "Spectrum Siemens";
		m_LutName[ LUT_TYPE_MITK_GRAY ] = "MITK gray";
		m_LutName[ LUT_TYPE_RED_BLUE ] = "Red-blue";
	}

}

vtkLookupTable * blLookupTables::BuildLutRedBlue()
{
	if( m_lutTable[ LUT_TYPE_RED_BLUE ] == NULL )
	{
		m_lutTable[ LUT_TYPE_RED_BLUE ] = vtkSmartPointer<vtkLookupTable>::New();
		m_lutTable[ LUT_TYPE_RED_BLUE ]->SetNumberOfTableValues( 256 );
		m_lutTable[ LUT_TYPE_RED_BLUE ]->SetTableRange( -1024.0, 1024.0 );
		m_lutTable[ LUT_TYPE_RED_BLUE ]->Build();
		for ( int i = 0 ; i < 128 ;i++ )
		{
			double redFloat = (double) 0/255.0;
			double greenFloat = (double) 0/255.0;
			double blueFloat = (double) (i*2)/255.0;
			m_lutTable[ LUT_TYPE_RED_BLUE ]->SetTableValue( 128 - i, redFloat, greenFloat, blueFloat, 1);
		}
		for ( int i = 0 ; i < 128 ;i++ )
		{
			double redFloat = (double) (i*2)/255.0;
			double greenFloat = (double) 0/255.0;
			double blueFloat = (double) 0/255.0;
			m_lutTable[ LUT_TYPE_RED_BLUE ]->SetTableValue( i + 128, redFloat, greenFloat, blueFloat, 1);
		}

	}

	return m_lutTable[ LUT_TYPE_RED_BLUE ];
}

