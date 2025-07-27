00001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example09 09 - adding user information to a fastjet::PseudoJet
00004 ///
00005 /// This example illustrates how it is possible to associate
00006 /// user-defined information to a fastjet::PseudoJet (beyond the
00007 /// simple user index), using a class derived from
00008 /// fastjet::UserInfoBase.
00009 ///
00010 /// Note that in this example we have chosen to use this
00011 /// user-defined information to obtain properties of the constituents
00012 /// of the reconstructed jet (e.g. if the event is made of a hard
00013 /// interaction and pileup, what part of the reconstructed jet comes
00014 /// from the hard interaction). To do that, we also show how to
00015 /// introduce a user-defined fastjet::Selector. For some applications,
00016 /// it might also be useful to define new recombination schemes using
00017 /// the extra information.
00018 ///
00019 /// run it with    : ./09-user_info < data/Pythia-dijet-ptmin100-lhc-pileup-1ev.dat
00020 ///
00021 /// (Note that this event consists of many sub-events, the first one
00022 /// being the "hard" interaction and the following being minbias
00023 /// events composing the pileup. It has the specificity that it also
00024 /// contains the PDG id of the particles)
00025 ///
00026 /// Source code: 09-user_info.cc
00027 //----------------------------------------------------------------------
00028 
00029 //STARTHEADER
00030 // $Id: 09-user_info.cc 2692 2011-11-14 16:27:44Z soyez $
00031 //
00032 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00033 //
00034 //----------------------------------------------------------------------
00035 // This file is part of FastJet.
00036 //
00037 //  FastJet is free software; you can redistribute it and/or modify
00038 //  it under the terms of the GNU General Public License as published by
00039 //  the Free Software Foundation; either version 2 of the License, or
00040 //  (at your option) any later version.
00041 //
00042 //  The algorithms that underlie FastJet have required considerable
00043 //  development and are described in hep-ph/0512210. If you use
00044 //  FastJet as part of work towards a scientific publication, please
00045 //  include a citation to the FastJet paper.
00046 //
00047 //  FastJet is distributed in the hope that it will be useful,
00048 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00049 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00050 //  GNU General Public License for more details.
00051 //
00052 //  You should have received a copy of the GNU General Public License
00053 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00054 //----------------------------------------------------------------------
00055 //ENDHEADER
00056 
00057 #include "fastjet/ClusterSequence.hh"
00058 #include "fastjet/Selector.hh"
00059 #include <iostream> // needed for io
00060 #include <sstream>  // needed for io
00061 #include <cstdio>   // needed for io
00062 
00063 using namespace std;
00064 using namespace fastjet;
00065 
00066 //------------------------------------------------------------------------
00067 // the user information
00068 // 
00069 // To associate extra information to a PseudoJet, one first has to
00070 // create a class, derived from UserInfoBase, that contains
00071 // that information.
00072 //
00073 // In our simple example, we shall use 2 informations
00074 //  - the PDG id associated with the particle
00075 //  - the "vertex number" associated with the particle
00076 class MyUserInfo : public PseudoJet::UserInfoBase{
00077 public:
00078   // default ctor
00079   //  - pdg_id        the PDG id of the particle
00080   //  - vertex_number theid of the vertex it originates from
00081   MyUserInfo(const int & pdg_id_in, const int & vertex_number_in) :
00082     _pdg_id(pdg_id_in), _vertex_number(vertex_number_in){}
00083 
00084   /// access to the PDG id
00085   int pdg_id() const { return _pdg_id;}
00086   
00087   /// access to the vertex number
00088   int vertex_number() const { return _vertex_number;}
00089   
00090 protected:
00091   int _pdg_id;         // the associated pdg id
00092   int _vertex_number;  // the associated vertex number
00093 };
00094 
00095 
00096 //------------------------------------------------------------------------
00097 // Select pi0 and photons
00098 //
00099 // This shows how we can build a Selector that uses the user-defined
00100 // information to select particles that are either pi0's or photons
00101 // (we choose this purely for simplicity).
00102 // 
00103 // To create a user-defined Selector, the first step is to
00104 // create its associated "worker" class, i.e. to derive a class from
00105 // SelectorWorker. Then (see below), we just write a function
00106 // (SelectorIsPi0Gamma()) that creates a Selector with the
00107 // appropriate worker class.
00108 class SW_IsPi0Gamma : public SelectorWorker{
00109 public:
00110   // default ctor
00111   SW_IsPi0Gamma(){}
00112 
00113   // the selector's description
00114   string description() const{
00115     return "neutral pions or photons"; 
00116   }
00117 
00118   // keeps the ones that have the pdg id of the pi0
00119   bool pass(const PseudoJet &p) const{
00120     // This is how we access the extra information associated with the
00121     // particles we test.
00122     //   p.user_info<MyUserInfo>()
00123     // returns a reference to the user-defined information (of type
00124     // MyUserInfo, as mentioned explicitly). It ensures automatically
00125     // that there is an associated user info compatible with the
00126     // requested type (and throws an error if it is not the case)
00127     //
00128     // We can then access the "pdg_id" member of MyUserInfo to
00129     // extract the targeted information.
00130     const int & pdgid = p.user_info<MyUserInfo>().pdg_id();
00131     return (pdgid == 111) || (pdgid == 22);
00132   }
00133 };
00134 
00135 // the function that allows to write simply
00136 //    Selector sel = SelectorIsPi0Gamma();
00137 Selector SelectorIsPi0Gamma(){
00138   return Selector(new SW_IsPi0Gamma());
00139 }
00140 
00141 //------------------------------------------------------------------------
00142 // Select particles from a given vertex number
00143 //
00144 // This is the same kind of construct as just above except that we
00145 // select on particles that are originated from a given vertex. The
00146 // test event has been structured as a superposition of sub-events
00147 // (the 0th being the hard interaction) and each particle will be
00148 // associated a vertex number. This Selector allows to select
00149 // particles corresponding to a given vertex number.
00150 //
00151 // As in the previous case, we start with the worker class and then
00152 // write a function for the Selector itself.
00153 class SW_VertexNumber : public SelectorWorker{
00154 public:
00155   // ctor from the vertex we want to keep
00156   SW_VertexNumber(const int & vertex_number) : _vertex_number(vertex_number){}
00157 
00158   // the selector's description
00159   string description() const{
00160     ostringstream oss;
00161     oss << "vertex number " << _vertex_number;
00162     return oss.str();
00163   }
00164 
00165   // keeps the ones that have the correct vertex number
00166   bool pass(const PseudoJet &p) const{
00167     // This is how we access the extra information associated with the
00168     // particles we test.
00169     //   p.user_info<MyUserInfo>()
00170     // returns a reference to the user-defined information (of type
00171     // MyUserInfo, as mentioned explicitly). It ensures automatically
00172     // that there is an associated user info compatible with the
00173     // requested type (and throws an error if it is not the case)
00174     //
00175     // We can then access the "vertex_number" member of MyUserInfo to
00176     // extract the targeted information.
00177     return p.user_info<MyUserInfo>().vertex_number()==_vertex_number;
00178   }
00179 
00180 private:
00181   int _vertex_number;  // the vertex number we're selecting
00182 };
00183 
00184 // The function that allows to write e.g.
00185 //  Selector sel = !SelectorVertexNumber(0);
00186 // to select particles from all vertices except the 0th.
00187 Selector SelectorVertexNumber(const int & vertex_number){
00188   return Selector(new SW_VertexNumber(vertex_number));
00189 }
00190 
00191 
00192 //------------------------------------------------------------------------
00193 // The example code associating user-info to the particles in the event
00194 int main(){
00195   // read in input particles
00196   //----------------------------------------------------------
00197   vector<PseudoJet> input_particles;
00198   
00199   double px, py , pz, E;
00200   string str;
00201   int vertex_number=-1;
00202   int pdg_id = 21;
00203   while (getline(cin, str)){
00204     // if the line does not start with #, it's a particle
00205     // read its momentum and pdg id
00206     if (str[0] != '#'){
00207       istringstream iss(str);
00208       if (! (iss >> px >> py >> pz >> E >> pdg_id)){
00209         cerr << "Wrong file format: particles must be specified with" << endl;
00210         cerr << "  px py pz E pdg_id" << endl;
00211       }
00212 
00213       // first create a PseudoJet with the correct momentum
00214       PseudoJet p(px,py,pz,E);
00215 
00216       // associate to that our user-defined extra information
00217       // which is done using 
00218       //   PseudoJet::set_user_info()
00219       //
00220       // IMPORTANT NOTE: set_user_info(...) takes a pointer as an
00221       // argument. It will "own" that pointer i.e. will delete it when
00222       // all the PseudoJet's using it will be deleted.
00223       //
00224       // NB: once you've done p.set_user_info(my_user_info_ptr), you must
00225       // not call p2.set_user_info(my_user_info_ptr) with the same pointer
00226       // because p and p2 will both attempt to delete it when they go out
00227       // of scope causing a double-free corruption error. Instead do
00228       // p2.user_info_shared_ptr() = p.user_info_shared_ptr();
00229       p.set_user_info(new MyUserInfo(pdg_id, vertex_number));
00230       PseudoJet p2; // defined only to make the above documentation consistent!
00231 
00232       input_particles.push_back(p);
00233       continue;
00234     }
00235 
00236     // check if we start a new sub-event
00237     if ((str.length()>=9) && (str.compare(0,9,"#SUBSTART")==0)){
00238       vertex_number++;
00239     }
00240   }
00241   
00242 
00243   // create a jet definition: 
00244   // a jet algorithm with a given radius parameter
00245   //----------------------------------------------------------
00246   double R = 0.6;
00247   JetDefinition jet_def(antikt_algorithm, R);
00248 
00249 
00250   // run the jet clustering with the above jet definition
00251   //----------------------------------------------------------
00252   ClusterSequence clust_seq(input_particles, jet_def);
00253 
00254 
00255   // get the resulting jets ordered in pt
00256   //----------------------------------------------------------
00257   double ptmin = 25.0;
00258   vector<PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00259 
00260 
00261   // tell the user what was done
00262   //  - the description of the algorithm used
00263   //  - extract the inclusive jets with pt > 5 GeV
00264   //    show the output as 
00265   //      {index, rap, phi, pt}
00266   //----------------------------------------------------------
00267   cout << "Ran " << jet_def.description() << endl;
00268 
00269   // label the columns
00270   printf("%5s %15s %15s %15s %15s %15s\n","jet #",
00271          "rapidity", "phi", "pt",
00272          "pt_hard", "pt_pi0+gamma");
00273 
00274   // a selection on the 1st vertex
00275   Selector sel_vtx0 = SelectorVertexNumber(0);
00276 
00277   // a selection on the pi0
00278   Selector sel_pi0gamma = SelectorIsPi0Gamma();
00279 
00280   // print out the details for each jet
00281   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00282     const PseudoJet & full = inclusive_jets[i];
00283     const vector<PseudoJet> constituents = full.constituents();
00284 
00285     // get the contribution from the 1st vertex
00286     PseudoJet hard = join(sel_vtx0(constituents));
00287     
00288     // get the contribution from the pi0's
00289     PseudoJet pi0gamma = join(sel_pi0gamma(constituents));
00290     
00291     // print the result
00292     printf("%5u %15.8f %15.8f %15.8f %15.8f %15.8f\n", i,
00293            full.rap(), full.phi(), full.perp(),
00294            hard.perp(), pi0gamma.perp());
00295   }
00296 
00297   return 0;
00298 }