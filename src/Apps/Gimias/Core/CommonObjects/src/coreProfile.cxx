/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreProfile.h"

using namespace Core;

//!
Profile::Profile(void)
{
	this->defaultProfile = "Common Functionalities";
}

//!
Profile::~Profile(void)
{
	// void by now
}

//!
Profile::ProfileSet& Profile::GetProfileAsStringSet(void)
{
	return this->innerProfile;
}

/** Returns true if the User profile is of type a given actor type
* @param userType is a string identifying an actor type 
*/
bool Profile::ProfileHasType(const std::string& userType)
{
	return innerProfile.find(userType) != innerProfile.end();
}

/** Returns true if the User profile is of type a given Profile
* @param profile is a given Profile to match with the curent user profile
*/
bool Profile::ProfileIsOfType(Profile::Pointer profile)
{
	bool AinB;
	bool BinA;
	// Checks the antisymmetry property of the sets [(A = B) <==> (A in B) && (B in A)]
	AinB = this->ProfileIsASubsetOfType(profile);
	BinA = this->ProfileIsASupersetOfType(profile);
	return AinB && BinA;
}

//!
bool Profile::operator==(Profile::Pointer profile)
{
	return ProfileIsOfType(profile);
}


/** Returns true if this object is a subset of the profiles handled by the given user profile .
* Is a subset if all instances of A (this) are in B (the given profile)
* @param profile is a given Profile to match with the current user profile
*/
bool Profile::ProfileIsASubsetOfType(Profile::Pointer profile)
{
	bool BinA = true;
	Profile::ProfileSet::iterator it;
	Profile::ProfileSet profileSet = profile->GetProfileAsStringSet();
	
	// Check All B in A
	for(it = profileSet.begin(); BinA && it != profileSet.end(); ++it)
	{
		BinA &= this->innerProfile.find(*it) != this->innerProfile.end();
	}
	return BinA;
}

/** Returns true if this object is a superset of the profiles handled by the given user profile .
* Is a superset if all instances of B (the given profile) are in A (this)
* @param profile is a given Profile to match with the curent user profile
*/
bool Profile::ProfileIsASupersetOfType(Profile::Pointer profile)
{
	bool AinB = true;
	Profile::ProfileSet::iterator it;
	Profile::ProfileSet profileSet = profile->GetProfileAsStringSet();
	
	// Check All A in B
	for(it = this->innerProfile.begin(); AinB && it != this->innerProfile.end(); ++it)
	{
		AinB &= profileSet.find(*it) != profileSet.end();
	}
	return AinB;
}

/** Returns true if this object has at least one of the profiles handled by the given user profile .
* @param profile is a given Profile to match with the current user profile
*/
bool Profile::ProfileSharesSomeOfType(Profile::Pointer profile)
{
	bool sharesOne = false;
	Profile::ProfileSet::iterator it;
	Profile::ProfileSet profileSet = profile->GetProfileAsStringSet();
	
	// Check at least one of B in A
	for(it = profileSet.begin(); !sharesOne && it != profileSet.end(); ++it)
	{
		sharesOne = this->innerProfile.find(*it) != this->innerProfile.end();
	}
	return sharesOne;
}

/** Adds a Core::UserType element to the set of profiles */
void Profile::AddToProfile(const std::string& userType)
{
	this->innerProfile.insert(userType);
}

/** Merges all instances of the given profile into this one */
void Profile::MergeProfiles(Profile::Pointer profile)
{
	Profile::ProfileSet::iterator it;
	Profile::ProfileSet profileSet = profile->GetProfileAsStringSet();
	for(it = profileSet.begin(); it != profileSet.end(); ++it)
	{
		this->innerProfile.insert(*it);
	}
}

/** Remove a Core::UserType element from the set of active profiles */
void Profile::RemoveFromProfile(const std::string& userType)
{
	this->innerProfile.erase(userType);
}
		
/** Remove all elements from the set of active profiles */
void Profile::ClearProfile(void)
{
	this->innerProfile.clear();
}

/** 
Remove all elements from the set of active profiles and resets it to the default state.
This is, setting the profile only for "Common Functionalities"
*/
void Profile::ResetProfileToDefault(void)
{
	this->innerProfile.clear();
	this->AddToProfile(this->defaultProfile);
}

/** 
Convenience function that prompts all instances of the profile ina a comma separed string,
useful for displaying it to the user as simple text
*/
std::string Profile::GetNamesAsCommaSeparedString(void)
{
	std::string profileStr = "";
	Profile::ProfileSet::iterator it;
	for(it = this->innerProfile.begin(); it != this->innerProfile.end(); ++it)
	{
		profileStr += (*it) + ",";
	}
	// Remove the last comma
	if(profileStr.length() > 0) profileStr.erase(profileStr.length()-1, 1);
	return profileStr;
}
