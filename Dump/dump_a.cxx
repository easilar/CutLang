#include "dump_a.h"
#include "TParameter.h"
#include <TRandom.h>
#include "DBXNtuple.h"
#include "TFile.h"

int DumpdbxA::plotVariables(int sel) {
    dbxA::plotVariables (sel);
    //put your extra canvas,plots here
    return 0 ;  
}

int DumpdbxA:: readAnalysisParams() {
  int retval=0;
  TString CardName=cname;
          CardName+="-card.txt";

// cout << "We are here:"<< system ("pwd; ls -l")<<endl;
// cout << "Reading analysis parameters from:" << CardName << endl ;


//  minptm= ReadCard(CardName,"minptm",15.);
  TRGe  = ReadCard(CardName,"TRGe",1);
  TRGm  = ReadCard(CardName,"TRGm",1);

  ///// PUT ANALYSIS PARAMETERS INTO .ROOT //////////////
	
//  TParameter<double> *minptm_tmp=new TParameter<double> ("minptm", minptm);
  TParameter<double> *TRGe_tmp=new TParameter<double> ("TRGe", TRGe);
  TParameter<double> *TRGm_tmp=new TParameter<double> ("TRGm", TRGm);
	
 // minptm_tmp->Write("maxetam");
  TRGe_tmp->Write("TRGe");
  TRGm_tmp->Write("TRGm");

  return retval;
}

int DumpdbxA:: printEfficiencies() {
    int retval=0;
    PrintEfficiencies(eff);
    return retval;
}

int DumpdbxA:: initGRL() {
  int retval=0;
  grl_cut=true;
  return retval;
}

int DumpdbxA:: bookAdditionalHistos() {
//additional histograms are defined here
  hmmultextra   = new TH1F("hmmultextra" ,"Muon multiplicity",20,-0.5,19.5);
  hmptextra  = new TH1F("hmptextra" ,"Muon P_{T}",100,0.,600.);
  hmetaextra    = new TH1F("hmetaextra" ,"Muon #eta",100,-5.,5.);
  hmphiextra    = new TH1F("hmphiextra" ,"#phi of muons",100,-5.,5.);
  eff= new TH1F("eff","selection efficiencies",5,0.5,5.5);
  getInputs();

  return 0;
}

int DumpdbxA:: getInputs() {

        int retval=0;  
        ntsave = new DBXNtuple();
	ftsave = new TFile ("lvl0.root","RECREATE");
	ttsave = new TTree ("nt_tree", "saving data on the grid");
        ttsave->Branch("dbxAsave", ntsave);
	
        return retval;
}

/////////////////////////do not call this for this analysis//////////////////////
int DumpdbxA::makeAnalysis(vector<dbxMuon> muons, vector<dbxElectron> electrons, vector<dbxPhoton> photons,
                            vector<dbxJet> jets, TVector2 met, evt_data anevt) {
    map < int, TVector2 > mymap;
    TVector2 a; 
    mymap[1]=a;      
    vector <double> b;
    return makeAnalysis(muons,electrons,photons,jets,met,anevt,mymap, b); 
}
/////////////////////////////Analysis for Dump/////////////////////////////////
int DumpdbxA::makeAnalysis(vector<dbxMuon> muons, vector<dbxElectron> electrons, vector<dbxPhoton> photons,
         vector<dbxJet> jets, TVector2 met, evt_data anevt,map < int, TVector2 > met_syst_map, vector <double> uncs) {


         
//      here we save the DBXNTuple
ntsave->Clean();
ntsave->nEle=electrons.size();
	 for ( int i=0; i<(int)electrons.size(); i++) {
 		ntsave->nt_eles.push_back(electrons.at(i) );
                TLorentzVector ele4p=electrons.at(i).lv();
//                std::cout<<"E Pt = " << ele4p.Pt() << " eta = " << ele4p.Eta() << " phi = " << ele4p.Phi() << std::endl;
         }
ntsave->nMuo=muons.size();
	 for ( int i=0; i<(int)muons.size(); i++) {
		ntsave->nt_muos.push_back(muons.at(i) );
                TLorentzVector mu4p = muons.at(i).lv();
//                std::cout<<"M Pt = " << mu4p.Pt() << " eta = " << mu4p.Eta() << " phi = " << mu4p.Phi() << std::endl;
         }
ntsave->nJet=jets.size();
	 for ( int i=0; i<(int)jets.size(); i++) {
		 ntsave->nt_jets.push_back(jets.at(i) );
                 TLorentzVector jet4p = jets.at(i).lv();
//                 std::cout<<"J Pt = " << jet4p.Pt() << " eta = " << jet4p.Eta() << " phi = " << jet4p.Phi() << std::endl;
         }
	 for ( int i=0; i<(int)uncs.size(); i++) {
		 ntsave->nt_uncs.push_back(uncs.at(i) );
         }
for (map<int, TVector2>::iterator itm = met_syst_map.begin(); itm != met_syst_map.end(); itm++) {
   ntsave->nt_sys_met.push_back( (*itm).second );
//     cout << ((*itm).second).Mod() << "  " << ((*itm).second).Phi() << endl;
}
//     cout << "~~~~~~~~~~~~~~~~~~~~~ E:"<<TRGe <<"  M:"<<TRGm <<endl;


ntsave->nt_met=met;
ntsave->nt_evt=anevt;

ntsave->nt_muos.resize    ( muons.size()              );
ntsave->nt_eles.resize    ( electrons.size()          );
ntsave->nt_jets.resize    ( jets.size()               );
ntsave->nt_uncs.resize    ( uncs.size()               );

ttsave->Fill();

//cout <<"Fill finished.\n";
/////////////////////////////////////
//Cuts and additional analysis//////
/////////////////////////////////////

  int cur_cut=1;

  eff->GetXaxis()->SetBinLabel(cur_cut,"all");
  eff->Fill(cur_cut, anevt.mcevt_weight);
  cur_cut++;


// force no GRL for MC events
  if ((TRGm>1) || (TRGe>1)) { grl_cut=false; }

// add the goodrunsList xml for this analysis
  eff->GetXaxis()->SetBinLabel(cur_cut,"GRL");
//if ( grl_cut) {
//       bool pass = isGoodRun(anevt.run_no,anevt.lumiblk_no);
//       if ( !pass ) return cur_cut;
//}
  eff->Fill(cur_cut, anevt.mcevt_weight);
  cur_cut++;

//cout <<"Before histo filling.\n";

// Fill kinematics of muons before any cut
    
     hmmultextra->Fill(muons.size(), anevt.mcevt_weight);
    	 
     for (UInt_t i=0; i<muons.size(); i++) {
       	 hmptextra->Fill(fabs(muons.at(i).lv().Pt()), anevt.mcevt_weight);
  	 hmetaextra->Fill(muons.at(i).lv().Eta(),anevt.mcevt_weight);
   	 hmphiextra->Fill(muons.at(i).lv().Phi(), anevt.mcevt_weight);
     } 

//cout <<"Before calling mother class.\n";

// ............ do NOT remove .....
  dbxA::makeAnalysis (muons, electrons, photons, jets, met, anevt);
// ............ do NOT remove .....

for (int k=cur_cut; k<=eff->GetNbinsX(); k++) eff->Fill(k,anevt.mcevt_weight);
  return 0;

//cout <<"finished.\n";
}
//~~~~~~~~~~~~~~~
int DumpdbxA:: Finalize() {
        ftsave->Write();      
        ftsave->Close();
        return 0;
}