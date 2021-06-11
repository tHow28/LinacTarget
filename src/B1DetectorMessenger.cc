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
/// \file B2aDetectorMessenger.cc
/// \brief Implementation of the B2aDetectorMessenger class

#include "B1DetectorMessenger.hh"
#include "B1DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorMessenger::B1DetectorMessenger(B1DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fLinacDirectory = new G4UIdirectory("/Linac/");
  fLinacDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/Linac/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fTungstenThicknessCmd = new G4UIcmdWithADoubleAndUnit("/Linac/det/tungstenThickness",this);
  fTungstenThicknessCmd->SetGuidance("Define thickness of tungsten");
  fTungstenThicknessCmd->SetParameterName("tungstenThickness",false);
  fTungstenThicknessCmd->SetUnitCategory("Length");
  fTungstenThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fCopperThicknessCmd = new G4UIcmdWithADoubleAndUnit("/Linac/det/copperThickness", this);
  fCopperThicknessCmd->SetGuidance("Define thickness of copper");
  fCopperThicknessCmd->SetParameterName("copperThickness", false);
  fCopperThicknessCmd->SetUnitCategory("Length");
  fCopperThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorMessenger::~B1DetectorMessenger()
{
  delete fTungstenThicknessCmd;
  delete fCopperThicknessCmd;
  delete fLinacDirectory;
  delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fTungstenThicknessCmd )
   { fDetectorConstruction
        ->SetTungstenThickness(fTungstenThicknessCmd->GetNewDoubleValue(newValue));}

  if( command == fCopperThicknessCmd )
   { fDetectorConstruction
        ->SetCopperThickness(fCopperThicknessCmd->GetNewDoubleValue(newValue));}

//  if( command == fStepMaxCmd ) {
  //  fDetectorConstruction
    //  ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  //}   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
