//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file B1PlaneHit.hh
/// \brief Definition of the B1PlaneHit class
//

#ifndef B1PlaneHit_h
#define B1PlaneHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class B1PlaneHit : public G4VHit
{
public:
    B1PlaneHit();
    virtual ~B1PlaneHit();
    B1PlaneHit(const B1PlaneHit& right);
    const B1PlaneHit& operator=(const B1PlaneHit& right);
    int operator==(const B1PlaneHit& right) const;

    inline void* operator new(size_t);
    inline void  operator delete(void* hit);

    virtual void Print();
    virtual void Draw();

    // setter methods
    void SetLayerNumber(G4int number) { fLayerNumber = number; }
    void SetTime(G4double time) { fTime = time; }
    void SetPosition(G4ThreeVector position) { fPosition = position; }
    void AddEdep(G4double edep) { fEdep += edep; }

    // getter methods
    G4int          GetLayerNumber() const { return fLayerNumber; }
    G4double       GetTime() const { return fTime; }
    G4ThreeVector  GetPosition() const { return fPosition; }
    G4double GetEdep() const { return fEdep; }

private:
    // data members
    G4int          fLayerNumber;
    G4double       fTime;
    G4ThreeVector  fPosition;
    G4double       fEdep;
};

typedef G4THitsCollection<B1PlaneHit> B1PlaneHitsCollection;

extern G4Allocator<B1PlaneHit>* B1PlaneHitAllocator;

inline void* B1PlaneHit::operator new(size_t)
{
    if (!B1PlaneHitAllocator)
        B1PlaneHitAllocator = new G4Allocator<B1PlaneHit>;
    return (void*)B1PlaneHitAllocator->MallocSingle();
}

inline void B1PlaneHit::operator delete(void* hit)
{
    B1PlaneHitAllocator->FreeSingle((B1PlaneHit*)hit);
}

#endif



