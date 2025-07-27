00001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example05 05 - using e+e- algorithms
00004 ///
00005 /// illustrate the use of e^+ e^- algorithms
00006 ///
00007 /// They mostly differ from the pp algorithm by the fact that rather
00008 /// than using a radius parameter and inclusive jets, they use
00009 /// exclusive jets in one of the following ways:
00010 ///  - a fixed number of them
00011 ///  - with a dcut
00012 ///  - with a ycut
00013 ///
00014 /// Note that natively, FastJet includes the kt (ee_kt_algorithm) and
00015 /// genkt (ee_genkt_algorithm) algorithms. Others (like Cambridge for
00016 /// e+ e-, Jade or SISCone in spherical coordinates) are available as
00017 /// plugins (see 03-plugin.cc)
00018 ///
00019 /// run it with    : ./05-eplus_eminus < data/single-ee-event.dat
00020 ///
00021 /// Source code: 05-eplus_eminus.cc
00022 //----------------------------------------------------------------------
00023 
00024 //STARTHEADER
00025 // $Id: 05-eplus_eminus.cc 2684 2011-11-14 07:41:44Z soyez $
00026 //
00027 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00028 //
00029 //----------------------------------------------------------------------
00030 // This file is part of FastJet.
00031 //
00032 //  FastJet is free software; you can redistribute it and/or modify
00033 //  it under the terms of the GNU General Public License as published by
00034 //  the Free Software Foundation; either version 2 of the License, or
00035 //  (at your option) any later version.
00036 //
00037 //  The algorithms that underlie FastJet have required considerable
00038 //  development and are described in hep-ph/0512210. If you use
00039 //  FastJet as part of work towards a scientific publication, please
00040 //  include a citation to the FastJet paper.
00041 //
00042 //  FastJet is distributed in the hope that it will be useful,
00043 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00044 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00045 //  GNU General Public License for more details.
00046 //
00047 //  You should have received a copy of the GNU General Public License
00048 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00049 //----------------------------------------------------------------------
00050 //ENDHEADER
00051 
00052 #include "fastjet/ClusterSequence.hh"
00053 #include <iostream> // needed for io
00054 #include <cstdio>   // needed for io
00055 
00056 using namespace std;
00057 
00058 /// an example program showing how to use fastjet
00059 int main(){
00060   
00061   // read in input particles
00062   //----------------------------------------------------------
00063   vector<fastjet::PseudoJet> input_particles;
00064   
00065   double px, py , pz, E;
00066   while (cin >> px >> py >> pz >> E) {
00067     // create a fastjet::PseudoJet with these components and put it onto
00068     // back of the input_particles vector
00069     input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
00070   }
00071   
00072 
00073   // create a jet definition for the kt algorithm (note that one
00074   // should not specify an R value here)
00075   //----------------------------------------------------------
00076   fastjet::JetDefinition jet_def(fastjet::ee_kt_algorithm);
00077 
00078   // run the jet clustering with the above jet definition
00079   //----------------------------------------------------------
00080   fastjet::ClusterSequence clust_seq(input_particles, jet_def);
00081 
00082   // get 3 exclusive jets
00083   //----------------------------------------------------------
00084   int n = 3;
00085   vector<fastjet::PseudoJet> exclusive_jets = clust_seq.exclusive_jets(n);
00086 
00087 
00088   // tell the user what was done
00089   //  - the description of the algorithm used
00090   //  - extract the inclusive jets with pt > 5 GeV
00091   //    show the output as 
00092   //      {index, rap, phi, pt, number of constituents}
00093   //----------------------------------------------------------
00094   cout << "Ran " << jet_def.description() << endl;
00095 
00096   // label the columns
00097   printf("%5s %15s\n","jet #", "E");
00098  
00099   // print out the details for each jet
00100   for (unsigned int i = 0; i < exclusive_jets.size(); i++) {
00101     printf("%5u %15.8f\n",
00102            i, exclusive_jets[i].perp());
00103   }
00104 
00105   return 0;
00106 }