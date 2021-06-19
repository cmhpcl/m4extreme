// Copyright (c) 2017-2018 Extreme Computation Technology and Solutions, LLC 
// All rights reserved
// see file License.txt for license details
////////////////////////////////////////////////////////////////////////////

#if !defined(femap_femapdata_h_)
#define femap_femapdata_h_

//



struct femapMaterial {

  static const int femapMaterialBValMax = 10;
  static const int femapMaterialIValMax = 25;
  static const int femapMaterialMValMax = 200;

  int    id;
  int    type;
  int    subtype;
  std::string title;
  int    bval[femapMaterialBValMax];
  int    ival[femapMaterialIValMax];
  double mval[femapMaterialMValMax];
};

//



struct femapProperty {

  static const int femapPropertyFlagMax = 4;

  int            id;
  int            matId;
  int            type;
  std::string         title;
  int            flag[femapPropertyFlagMax];
  int            num_val;
  std::vector<double> value;

};

//



struct femapNode {
  
  static const int femapNodeXMax   = 3;
  static const int femapNodePermBC = 6;

  int    id;
  double x[femapNodeXMax];
  int permBc[femapNodePermBC];

};

//

struct femapElement {

  int id;
  int propId;
  int type;
  int topology;
  static const int tetra_10_nodes = 10;
  static const int tetra_4_nodes  = 6;
  int geomId;
  int formulation;  // int is a guess--documentation doesn't give type
  std::vector<int> node;

};

//

struct constraint {

  static const int femapConstraintDofMax = 6;

  int  id;
  int  dof[femapConstraintDofMax];
  int  ex_geom;

};

//

struct femapConstraintSet {

  int                id;
  std::string             title;
  std::vector<constraint> nodalConstraint;

};

//



struct load {

  static const int femapLoadDofMax   = 3;
  static const int femapLoadValueMax = 3;

  int              id;
  int              type;
  static const int typeNodalForce = 1;
  static const int typeNodalVelocity = 5;
  static const int typeNodalDisplacement = 3;

  int              dof_face[femapLoadDofMax];
  double           value[femapLoadValueMax];
  bool             is_expanded;

};

//



struct femapLoadSet {

  static const int femapLoadSetGravMax   = 6;
  static const int femapLoadSetOriginMax = 3;
  static const int femapLoadSetOmegaMax  = 3;

  int          id;
  std::string       title;
  double       defTemp;
  bool         tempOn;
  bool         gravOn;
  bool         omegaOn;
  double       grav[femapLoadSetGravMax];
  double       origin[femapLoadSetOriginMax];
  double       omega[femapLoadSetOmegaMax];
  std::vector<load> loads;

};

//

struct groupRule {
  int type; // see FEMAP Neutral File Format Documentation for type identification
  int startID;     
  int stopID; 
  int incID;
  int include;
};

//

struct groupList {
  int type;    // see FEMAP Neutral File Format Documentation for type identification
  std::vector<int> entityID;
};

//

struct femapGroup {
  
  static const int femapGroupLayerMax = 2;

  int id;
  short int need_eval;
  std::string title;
  int layer[femapGroupLayerMax];
  int layer_method;
  std::vector<groupRule> rules;
  std::vector<groupList> lists;

};

#endif // femap_femapdata_h_
