00001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example13 13 - boosted top tagging
00004 ///
00005 /// fastjet example program, illustration of carrying out boosted
00006 /// top subjet ID analysis using the Johns Hopkins top tagger as
00007 /// introduced in arXiv:0806.0848 (Kaplan, Rehermann, Schwartz
00008 /// and Tweedie)
00009 ///
00010 /// run it with    : ./13-boosted_top < data/boosted_top_event.dat
00011 ///
00012 /// Source code: 13-boosted_top.cc
00013 //----------------------------------------------------------------------
00014 
00015 
00016 //STARTHEADER
00017 // $Id: 13-boosted_top.cc 2684 2011-11-14 07:41:44Z soyez $
00018 //
00019 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00020 //
00021 //----------------------------------------------------------------------
00022 // This file is part of FastJet.
00023 //
00024 //  FastJet is free software; you can redistribute it and/or modify
00025 //  it under the terms of the GNU General Public License as published by
00026 //  the Free Software Foundation; either version 2 of the License, or
00027 //  (at your option) any later version.
00028 //
00029 //  The algorithms that underlie FastJet have required considerable
00030 //  development and are described in hep-ph/0512210. If you use
00031 //  FastJet as part of work towards a scientific publication, please
00032 //  include a citation to the FastJet paper.
00033 //
00034 //  FastJet is distributed in the hope that it will be useful,
00035 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00036 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00037 //  GNU General Public License for more details.
00038 //
00039 //  You should have received a copy of the GNU General Public License
00040 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00041 //----------------------------------------------------------------------
00042 //ENDHEADER
00043 
00044 #include <iostream> // needed for io
00045 #include <sstream>  // needed for internal io
00046 #include <iomanip>  
00047 #include <cmath>
00048 
00049 #include <fastjet/ClusterSequence.hh>
00050 #include <fastjet/Selector.hh>
00051 #include <fastjet/tools/JHTopTagger.hh>
00052 
00053 using namespace std;
00054 using namespace fastjet;
00055 
00056 
00057 //----------------------------------------------------------------------
00058 // forward declaration for printing out info about a jet
00059 //----------------------------------------------------------------------
00060 ostream & operator<<(ostream &, const PseudoJet &);
00061 
00062 //----------------------------------------------------------------------
00063 // core of the program
00064 //----------------------------------------------------------------------
00065 int main(){
00066 
00067   vector<PseudoJet> particles;
00068 
00069   // read in data in format px py pz E b-tag [last of these is optional]
00070   // lines starting with "#" are considered as comments and discarded
00071   //----------------------------------------------------------
00072   string line;
00073   while (getline(cin,line)) {
00074     if (line.substr(0,1) == "#") {continue;}
00075     istringstream linestream(line);
00076     double px,py,pz,E;
00077     linestream >> px >> py >> pz >> E;
00078 
00079     // construct the particle
00080     particles.push_back(PseudoJet(px,py,pz,E));
00081   }
00082 
00083   // compute the parameters to be used through the analysis
00084   // ----------------------------------------------------------
00085   double Et=0;
00086   for (unsigned int i=0; i<particles.size(); i++)
00087     Et += particles[i].perp();
00088 
00089   double R, delta_p, delta_r;
00090   if      (Et>2600){ R=0.4; delta_p=0.05; delta_r=0.19;}
00091   else if (Et>1600){ R=0.6; delta_p=0.05; delta_r=0.19;}
00092   else if (Et>1000){ R=0.8; delta_p=0.10; delta_r=0.19;}
00093   else{ cerr << "Et has to be at least 1 TeV"<< endl; return 1;}
00094 
00095   double ptmin = min(500.0, 0.7*Et/2);
00096 
00097   // find the jets
00098   // ----------------------------------------------------------
00099   JetDefinition jet_def(cambridge_algorithm, R);
00100   ClusterSequence cs(particles, jet_def);
00101   vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());
00102 
00103   cout << "Ran: " << jet_def.description() << endl << endl;
00104   cout << "2 Hardest jets: " << jets[0] << endl
00105        << "                " << jets[1] << endl << endl;
00106 
00107   if (jets[0].perp()<ptmin){
00108     cout << "No jet above the ptmin threshold" << endl;
00109     return 2;
00110   }
00111 
00112   // now do jet tagging using the Johns Hopkins top tagger
00113   // For simplicity, we just apply it to the hardest jet.
00114   //
00115   // In addition to delta_p and delta_r, note that there are two
00116   // further parameters to the JH top tagger that here are implicitly
00117   // set to their defaults:
00118   //
00119   // - cos_theta_W_max (defaults to 0.7) 
00120   // - mW (defaults to 80.4). 
00121   //
00122   // The value for mW implicitly assumes that momenta are passed in
00123   // GeV.
00124   // ----------------------------------------------------------
00125   JHTopTagger top_tagger(delta_p, delta_r);
00126   top_tagger.set_top_selector(SelectorMassRange(150,200));
00127   top_tagger.set_W_selector  (SelectorMassRange( 65, 95));
00128 
00129   PseudoJet tagged = top_tagger(jets[0]);
00130 
00131   cout << "Ran the following top tagger: " << top_tagger.description() << endl;
00132 
00133   if (tagged == 0){
00134     cout << "No top substructure found" << endl;
00135     return 0;
00136   }
00137 
00138   cout << "Found top substructure from the hardest jet:" << endl;
00139   cout << "  top candidate:     " << tagged << endl;
00140   cout << "  |_ W   candidate:  " << tagged.structure_of<JHTopTagger>().W() << endl;
00141   cout << "  |  |_  W subjet 1: " << tagged.structure_of<JHTopTagger>().W1() << endl;
00142   cout << "  |  |_  W subjet 2: " << tagged.structure_of<JHTopTagger>().W2() << endl;
00143   cout << "  |  cos(theta_W) =  " << tagged.structure_of<JHTopTagger>().cos_theta_W() << endl;
00144   cout << "  |_ non-W subjet:   " << tagged.structure_of<JHTopTagger>().non_W() << endl;
00145 }
00146 
00147 
00148 //----------------------------------------------------------------------
00149 // does the actual work for printing out a jet
00150 //----------------------------------------------------------------------
00151 ostream & operator<<(ostream & ostr, const PseudoJet & jet) {
00152   ostr << "pt, y, phi =" << setprecision(6)
00153        << " " << setw(9) << jet.perp() 
00154        << " " << setw(9)  <<  jet.rap()  
00155        << " " << setw(9)  <<  jet.phi()  
00156        << ", mass = " << setw(9) << jet.m();
00157   return ostr;
00158 }