/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blMatlabIO.h"
#include <fstream>
#include <vector>

void ToMatlab( const double* vector, int length, const char* filename, const char* var_name )
{
	std::ofstream theFile( filename );
	theFile<<var_name<<"=["<<std::endl;
	for( int i=0; i<length; i++ ) 
		theFile << vector[i] << " ;" << std::endl;
	theFile << "];" << std::endl;
	theFile.close();
}

void ToMatlab( const double* matrix, int rows, int cols, const char* filename, const char* var_name )
{
	std::ofstream theFile( filename );
	theFile<<var_name<<"=["<<std::endl;

	unsigned long index = 0;
	for(int row=0; row<rows; row++)
	{
		for( int col=0; col<cols; col++ ) 
		{
			theFile<<matrix[index++]<<"  ";
		}
		theFile<<";"<<std::endl;
	}

	theFile<<"];"<<std::endl;
	theFile.close();
}

void FromMatlab( blVector<double>::VectorType* vector, const char* filename )
{
	std::ifstream theFile( filename );
	char dump[1000];
	theFile>>dump;
	std::vector<double> temp_vector;

	while ( !theFile.eof() )
	{
		theFile>>dump;	
		if( dump[0]=='.' || dump[0]=='+' || dump[0]=='-' || ( dump[0]>='0' && dump[0]<='9' ) )
		{
			temp_vector.push_back( atof( dump ) );
		}
	}

	vector->set_size( temp_vector.size() );
	vector->copy_in( &temp_vector[0] );

	theFile.close();
}

void FromMatlab( vnl_matrix<double>* matrix, const char* filename )
{
	std::ifstream theFile( filename );
	char dump[1000];
	theFile>>dump;
	std::vector<double> temp_vector;
	int number_of_cols = 0;

	while ( !theFile.eof() )
	{
		theFile>>dump;	
		if( dump[0]=='.' || dump[0]=='+' || dump[0]=='-' || ( dump[0]>='0' && dump[0]<='9' ) )
		{
			temp_vector.push_back( atof( dump ) );
		}
		
		if( dump[0]==';' && number_of_cols==0)
		{
			number_of_cols = temp_vector.size();
		}
	}

	matrix->set_size( temp_vector.size()/number_of_cols, number_of_cols );
	matrix->copy_in( &temp_vector[0] );

	theFile.close();
}
