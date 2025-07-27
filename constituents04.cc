 Go to the documentation of this file.

00001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example04 04 - accessing clustering information in a PseudoJet
00004 ///
00005 /// illustrate how a jet can carry information about its clustering
00006 ///
00007 /// We do it by associating a user index to each of the input particles
00008 /// and show what particles are in each jets (above 5 GeV)
00009 ///
00010 /// We also illustrate a few other features about how a fastjet::PseudoJet
00011 /// can access its underlying structure.
00012 ///
00013 /// run it with    : ./04-constituents < data/single-event.dat
00014 ///
00015 /// Source code: 04-constituents.cc
00016 //----------------------------------------------------------------------
00017 
00018 //STARTHEADER
00019 // $Id: 04-constituents.cc 2704 2011-11-16 11:11:10Z soyez $
00020 //
00021 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00022 //
00023 //----------------------------------------------------------------------
00024 // This file is part of FastJet.
00025 //
00026 //  FastJet is free software; you can redistribute it and/or modify
00027 //  it under the terms of the GNU General Public License as published by
00028 //  the Free Software Foundation; either version 2 of the License, or
00029 //  (at your option) any later version.
00030 //
00031 //  The algorithms that underlie FastJet have required considerable
00032 //  development and are described in hep-ph/0512210. If you use
00033 //  FastJet as part of work towards a scientific publication, please
00034 //  include a citation to the FastJet paper.
00035 //
00036 //  FastJet is distributed in the hope that it will be useful,
00037 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00038 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00039 //  GNU General Public License for more details.
00040 //
00041 //  You should have received a copy of the GNU General Public License
00042 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00043 //----------------------------------------------------------------------
00044 //ENDHEADER
00045 
00046 #include "fastjet/ClusterSequence.hh"
00047 #include <iostream> // needed for io
00048 #include <cstdio>   // needed for io
00049 
00050 using namespace std;
00051 
00052 /// an example program showing how to use fastjet
00053 int main(){
00054   
00055   // read in input particles
00056   //----------------------------------------------------------
00057   vector<fastjet::PseudoJet> input_particles;
00058   
00059   valarray<double> fourvec(4);
00060   int index=0;
00061   while (cin >> fourvec[0] >> fourvec[1] >> fourvec[2] >> fourvec[3]) {
00062     // create a particle with the approprite 4-momentum and 
00063     // set its user index to keep track of its index.
00064     // you can construct a PseudoJet from any object that allows subscripts
00065     // from [0] .. [3] (the last one must be the energy)
00066     fastjet::PseudoJet particle(fourvec);
00067 
00068     particle.set_user_index(index);
00069     input_particles.push_back(particle); 
00070 
00071     index++;
00072   }
00073   
00074 
00075   // create a jet definition: 
00076   // a jet algorithm with a given radius parameter
00077   //----------------------------------------------------------
00078   double R = 0.6;
00079   fastjet::JetDefinition jet_def(fastjet::kt_algorithm, R);
00080 
00081 
00082   // run the jet clustering with the above jet definition
00083   //----------------------------------------------------------
00084   fastjet::ClusterSequence clust_seq(input_particles, jet_def);
00085 
00086 
00087   // get the resulting jets ordered in pt
00088   //----------------------------------------------------------
00089   double ptmin = 5.0;
00090   vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00091 
00092 
00093   // tell the user what was done
00094   //  - the description of the algorithm used
00095   //  - extract the inclusive jets with pt > 5 GeV
00096   //    show the output as 
00097   //      {index, rap, phi, pt, number of constituents}
00098   //----------------------------------------------------------
00099   cout << "Ran " << jet_def.description() << endl << endl;
00100 
00101   // label the columns
00102   printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt", "n constituents");
00103   printf("        indices of constituents\n\n");
00104  
00105   // print out the details for each jet
00106   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00107     // get the constituents of the jet
00108     vector<fastjet::PseudoJet> constituents = inclusive_jets[i].constituents();
00109 
00110     printf("%5u %15.8f %15.8f %15.8f %8u\n",
00111            i, inclusive_jets[i].rap(), inclusive_jets[i].phi(),
00112            inclusive_jets[i].perp(), (unsigned int) constituents.size());
00113 
00114     printf("       ");
00115     for (unsigned int j=0; j<constituents.size(); j++){
00116       printf("%4u ", constituents[j].user_index());
00117       if (j%10==9) printf("\n       ");
00118     }
00119     printf("\n\n");
00120   }
00121 
00122   return 0;
00123 }
