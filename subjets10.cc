00001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example10 10 - extracting subjets
00004 ///
00005 /// fastjet example program to show how to access subjets;
00006 /// 
00007 /// See also 12-boosted_higgs.cc to see the use of subjets for
00008 /// identifying boosted higgs (and other objects)
00009 ///
00010 /// run it with    : ./10-subjets < data/single-event.dat
00011 ///
00012 /// Source code: 10-subjets.cc
00013 //----------------------------------------------------------------------
00014 
00015 //STARTHEADER
00016 // $Id: 10-subjets.cc 2742 2011-11-22 22:27:08Z salam $
00017 //
00018 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00019 //
00020 //----------------------------------------------------------------------
00021 // This file is part of FastJet.
00022 //
00023 //  FastJet is free software; you can redistribute it and/or modify
00024 //  it under the terms of the GNU General Public License as published by
00025 //  the Free Software Foundation; either version 2 of the License, or
00026 //  (at your option) any later version.
00027 //
00028 //  The algorithms that underlie FastJet have required considerable
00029 //  development and are described in hep-ph/0512210. If you use
00030 //  FastJet as part of work towards a scientific publication, please
00031 //  include a citation to the FastJet paper.
00032 //
00033 //  FastJet is distributed in the hope that it will be useful,
00034 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00035 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00036 //  GNU General Public License for more details.
00037 //
00038 //  You should have received a copy of the GNU General Public License
00039 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00040 //----------------------------------------------------------------------
00041 //ENDHEADER
00042 
00043 #include "fastjet/ClusterSequence.hh"
00044 #include <iostream> // needed for io
00045 #include <cstdio>   // needed for io
00046 
00047 using namespace std;
00048 using namespace fastjet;
00049 
00050 int main(){
00051   
00052   // read in input particles
00053   //----------------------------------------------------------
00054   vector<PseudoJet> input_particles;
00055   
00056   double px, py , pz, E;
00057   while (cin >> px >> py >> pz >> E) {
00058     // create a PseudoJet with these components and put it onto
00059     // back of the input_particles vector
00060     input_particles.push_back(PseudoJet(px,py,pz,E)); 
00061   }
00062   
00063 
00064   // create a jet definition: 
00065   // for subjet studies, Cambridge/Aachen is the natural algorithm
00066   //----------------------------------------------------------
00067   double R = 1.0;
00068   JetDefinition jet_def(cambridge_algorithm, R);
00069 
00070 
00071   // run the jet clustering with the above jet definition
00072   // and get the jets above 5 GeV
00073   //----------------------------------------------------------
00074   ClusterSequence clust_seq(input_particles, jet_def);
00075   double ptmin = 6.0;
00076   vector<PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00077 
00078   // extract the subjets at a smaller angular scale (Rsub=0.5)
00079   //
00080   // This is done by using ClusterSequence::exclusive_subjets(dcut):
00081   // for the Cambridge/Aachen algorithm, running with R and then
00082   // asking for exclusive subjets with dcut should give the same
00083   // subjets as rerunning the algorithm with R'=R*sqrt(dcut) on the
00084   // jet's constituents.
00085   //
00086   // At the same time we output a summary of what has been done and the 
00087   // resulting subjets
00088   //----------------------------------------------------------
00089   double Rsub = 0.5;
00090   double dcut = pow(Rsub/R,2);
00091 
00092   // a "header" for the output
00093   cout << "Ran " << jet_def.description() << endl;
00094   cout << "Showing the jets above " << ptmin << " GeV" << endl;
00095   cout << "And their subjets for Rsub = " << Rsub << endl;
00096   printf("%10s %15s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt", "n constituents");
00097 
00098   // show the jets and their subjets
00099   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00100     // get the subjets
00101     vector<PseudoJet> subjets = sorted_by_pt(inclusive_jets[i].exclusive_subjets(dcut));
00102 
00103     cout << endl;
00104     // print the jet and its subjets
00105     printf("%5u      %15.8f %15.8f %15.8f %8d\n", i, 
00106            inclusive_jets[i].rap(), inclusive_jets[i].phi(),
00107            inclusive_jets[i].perp(), int(inclusive_jets[i].constituents().size()));
00108 
00109     for (unsigned int j=0; j<subjets.size(); j++)
00110       printf("   sub%4u %15.8f %15.8f %15.8f %8u\n", j, 
00111              subjets[j].rap(), subjets[j].phi(),
00112              subjets[j].perp(), 
00113              (unsigned int) subjets[j].constituents().size());
00114   }
00115 
00116   return 0;
00117 }