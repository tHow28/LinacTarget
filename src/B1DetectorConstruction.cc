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
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "B1DetectorMessenger.hh"
#include "B1TungstenSD.hh"
#include "B1CopperSD.hh"
#include "B1PlaneSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  fScoringVolume2(0),
  fPlane(0),
  flogicEnv(0),
  fTungstenThickness(NULL),
  fCopperThickness(NULL)
{ 
    fMessenger = new B1DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{
    delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
    //Define volumes
    return DefineVolumes();
}

G4VPhysicalVolume* B1DetectorConstruction::DefineVolumes()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 10*cm, env_sizeZ = 15*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  flogicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    flogicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //     
  // Shape 1 - tungsten disc
  //  
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_W");
        
  // Cylinder shape       
  G4double shape1_rmin = 0. * mm;
  G4double shape1_rmax = 5. * mm;
  G4double fTungstenThickness = 0.585 * mm;
  //if (shape1_hz != fTungstenThickness) {
  //    if (fTungstenThickness > 0) {
  //        shape1_hz = fTungstenThickness;
  //    }
  //}
  //G4cout << G4endl << "--> Tungsten is " << shape1_hz * 2 << " mm thick." << G4endl;
  G4double shape1_phimin = 0.*deg, shape1_phimax = 360.*deg;
  G4Tubs* solidShape1 =    
    new G4Tubs("Shape1", 
    shape1_rmin, shape1_rmax, fTungstenThickness, shape1_phimin, shape1_phimax);
                      

  fScoringVolume =                         
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name


  G4ThreeVector pos1 = G4ThreeVector(0., 0., -1.*fTungstenThickness);
  G4VPhysicalVolume* fTungstenPhys = 
    new G4PVPlacement(0,                       //no rotation
                      pos1,                    //at position
                      fScoringVolume,             //its logical volume
                      "Shape1",                //its name
                      flogicEnv,                //its mother  volume
                      false,                   //no boolean operation
                      0,                       //copy number
                      checkOverlaps);          //overlaps checking

  //     
  // Shape 2 - copper disc
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Cu");
  

  // Cylinder shape       
  G4double shape2_rmin = 0. * mm;
  G4double shape2_rmax = 5. * mm;
  G4double shape2_hz = 0.75 * mm;
  G4double shape2_phimin = 0. * deg, shape2_phimax = 360. * deg;
  G4Tubs* solidShape2 =    
    new G4Tubs("Shape2",                      //its name
    shape2_rmin, shape2_rmax, shape2_hz, shape2_phimin, shape2_phimax);
                
  fScoringVolume2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                        shape2_mat,          //its material
                        "Shape2");           //its name
               
  G4ThreeVector pos2 = G4ThreeVector(0, 0, shape2_hz);
  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    fScoringVolume2,             //its logical volume
                    "Shape2",                //its name
                    flogicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                
  // Set Shape2 as scoring volume
  //
  //fScoringVolume  = logicShape1;
  //fScoringVolume2 = logicShape2;


  //Box shape for non-interacting detector

  G4double plane_hx = 50. * mm;
  G4double plane_hy = 50. * mm;
  G4double plane_hz = .05 * mm;

  G4Material* air = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidPlane = 
      new G4Box("Plane",                    //its name
          plane_hx, plane_hy, plane_hz); //its size

  fPlane =
      new G4LogicalVolume(solidPlane,            //its solid
          air,             //its material
          "Plane");         //its name

  new G4PVPlacement(0,                       //no rotation
      G4ThreeVector(0, 0, 10* mm),         //at (0,0,0)
      fPlane,                //its logical volume
      "Plane",              //its name
      logicWorld,              //its mother  volume
      false,                   //no boolean operation
      0,                       //copy number
      checkOverlaps);          //overlaps checking

   // Greate the region
  G4Region* planeRegion = new G4Region("PlaneRegion");

   // Define cuts object for the new region and set values
   G4ProductionCuts* cuts = new G4ProductionCuts;
   cuts->SetProductionCut(0.01 * mm); // for gamma, e+, e-, p
   // Assign cuts to region
   planeRegion->SetProductionCuts(cuts);

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    // 
    // Sensitive detectors
    //
    // tungsten target layer
    B1TungstenSD* tungstenSD
        = new B1TungstenSD("TungstenSD", "TungstenHitsCollection", 0);
    G4SDManager::GetSDMpointer()->AddNewDetector(tungstenSD);
    SetSensitiveDetector("Shape1", tungstenSD);

    // copper target layer
    B1CopperSD* CopperSD
        = new B1CopperSD("CopperSD", "CopperHitsCollection", 0);
    G4SDManager::GetSDMpointer()->AddNewDetector(CopperSD);
    SetSensitiveDetector("Shape2", CopperSD);

    // non-interacting plane
    B1PlaneSD* planeSD
        = new B1PlaneSD("PlaneSD", "PlaneHitsCollection", 0);
    G4SDManager::GetSDMpointer()->AddNewDetector(planeSD);
    SetSensitiveDetector("Plane", planeSD);
}

void B1DetectorConstruction::SetTungstenThickness(G4double tungstenThickness)
{
    G4double newWthickness = tungstenThickness / 2;
    if (fTungstenThickness != newWthickness) {
        if (newWthickness) {
            // Get nist material manager
            G4NistManager* nist = G4NistManager::Instance();
            G4bool checkOverlaps = true;

            fTungstenThickness = newWthickness;
            G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_W");

            // Cylinder shape       
            G4double shape1_rmin = 0. * mm;
            G4double shape1_rmax = 5. * mm;
        
            G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
            G4Tubs* solidShape1 =
                new G4Tubs("Shape1",
                    shape1_rmin, shape1_rmax, fTungstenThickness, shape1_phimin, shape1_phimax);


            fScoringVolume =
                new G4LogicalVolume(solidShape1,         //its solid
                    shape1_mat,          //its material
                    "Shape1");           //its name


            G4ThreeVector pos1 = G4ThreeVector(0., 0., -1. * fTungstenThickness);
            G4VPhysicalVolume* fTungstenPhys = 
                new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    fScoringVolume,             //its logical volume
                    "Shape1",                //its name
                    flogicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
            G4cout << G4endl << "New W thickness: " << tungstenThickness << " mm." << G4endl;
        }
    }
}

void B1DetectorConstruction::SetCopperThickness(G4double copperThickness)
{
    fCopperThickness = copperThickness / 2;
}