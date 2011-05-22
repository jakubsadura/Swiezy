/*
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


template<>
void DataHolder<Core::SmartPointerObject::Pointer>::SetSubject( const Core::SmartPointerObject::Pointer& data, bool bForceNotification )
{
	if ( m_Data.IsNotNull( ) )
	{
		m_Data->RemoveObserverOnModified< DataHolder<Core::SmartPointerObject::Pointer> >( 
			this,
			&DataHolder<Core::SmartPointerObject::Pointer>::OnModifiedSubject );
	}

	doSetSubject( data, bForceNotification );

	if ( data.IsNotNull( ) )
	{
		data->AddObserverOnModified< DataHolder<Core::SmartPointerObject::Pointer> >( 
			this,
			&DataHolder<Core::SmartPointerObject::Pointer>::OnModifiedSubject );
	}
}


