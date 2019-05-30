/*
 * LoaderASE_t.h
 *
 *  Created on: Apr 3, 2019
 *      Author: tim
 */

#ifndef OBJ3D_H_
#define OBJ3D_H_

#include <string>
#include <map>
#include <vector>

#define TRIM_CHARS "\t\n\v\f\r "

typedef struct
{
  float R;
  float G;
  float B;
} RGBf;

typedef struct
{
  int I1;
  int I2;
  int I3;
} Int3;

typedef struct
{
  float f1;
  float f2;
  float f3;
} Float3;

typedef struct
{
  float X;
  float Y;
  float Z;
} Vector3f;

typedef struct
{
  int A;
  int B;
  int C;
  int SmoothGroup;
  int MtlId;
} MeshFace_t;

typedef struct MeshTFace
{
  int A;
  int B;
  int C;
} MeshTFace_t;

class ASEParser_t
{
public:
  typedef enum { pOk = 0, pUnknownToken, pUnimplemented, pDone} pError_t;
protected:
  typedef pError_t (ASEParser_t::*OnKeyWord_t)(std::vector<std::string> &);
  typedef std::map<std::string, OnKeyWord_t> Lexems_t;
  std::vector<ASEParser_t*> * Objs;
  virtual Lexems_t & Lexems(void) = 0;
  std::string& ltrim(std::string& str, const std::string& chars = TRIM_CHARS);
  std::string& rtrim(std::string& str, const std::string& chars = TRIM_CHARS);
  std::string& trim(std::string& str, const std::string& chars = TRIM_CHARS);
  std::vector<std::string> split(const std::string &s);
  pError_t Skip(std::vector<std::string> & args){ return pOk; };
public:
  ASEParser_t(std::vector<ASEParser_t*> * pObjs) { Objs = pObjs; }
  virtual ~ASEParser_t(){};
  pError_t ProcessLine(std::ifstream & fs);
};

class Scene_t: public ASEParser_t
{
private:
  pError_t SetFileName(std::vector<std::string> & args);
  pError_t SetSceneFirstFrame(std::vector<std::string> & args);
  pError_t SetSceneLastFrame(std::vector<std::string> & args);
  pError_t SetSceneFrameSpeed(std::vector<std::string> & args);
  pError_t SetSceneTicksPerFrame(std::vector<std::string> & args);
  pError_t SetSceneBackgroundStatic(std::vector<std::string> & args);
  pError_t SetSceneAmbientStatic(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*SCENE_FILENAME", (OnKeyWord_t)&Scene_t::SetFileName},
      {"*SCENE_FIRSTFRAME", (OnKeyWord_t)&Scene_t::SetSceneFirstFrame},
      {"*SCENE_LASTFRAME", (OnKeyWord_t)&Scene_t::SetSceneLastFrame},
      {"*SCENE_FRAMESPEED", (OnKeyWord_t)&Scene_t::SetSceneFrameSpeed},
      {"*SCENE_TICKSPERFRAME", (OnKeyWord_t)&Scene_t::SetSceneTicksPerFrame},
      {"*SCENE_BACKGROUND_STATIC", (OnKeyWord_t)&Scene_t::SetSceneBackgroundStatic},
      {"*SCENE_AMBIENT_STATIC", (OnKeyWord_t)&Scene_t::SetSceneAmbientStatic}
   };

protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };
public:
  Scene_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}
  std::string FileName;
  int SceneFirstFrame;
  int SceneLastFrame;
  int SceneFrameSpeed;
  int SceneTicksPerFrame;
  RGBf SceneBackgroundStatic;
  RGBf SceneAmbientStatic;
};

class MaterialMap_t: public ASEParser_t
{
private:
  pError_t SetMapName(std::vector<std::string> & args);
  pError_t SetMapClass(std::vector<std::string> & args);
  pError_t SetMapSubno(std::vector<std::string> & args);
  pError_t SetMapAmount(std::vector<std::string> & args);
  pError_t SetBitmap(std::vector<std::string> & args);
  pError_t SetMapType(std::vector<std::string> & args);
  pError_t SetUvwUOffset(std::vector<std::string> & args);
  pError_t SetUvwVOffset(std::vector<std::string> & args);
  pError_t SetUvwUTiling(std::vector<std::string> & args);
  pError_t SetUvwVTiling(std::vector<std::string> & args);
  pError_t SetUvwAngle(std::vector<std::string> & args);
  pError_t SetUvwBlur(std::vector<std::string> & args);
  pError_t SetUvwBlurOffset(std::vector<std::string> & args);
  pError_t SetUvwNouseAmt(std::vector<std::string> & args);
  pError_t SetUvwNoiseSize(std::vector<std::string> & args);
  pError_t SetUvwNoiseLevel(std::vector<std::string> & args);
  pError_t SetUvwNoisePhase(std::vector<std::string> & args);
  pError_t SetBitmapFilter(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MAP_NAME", (OnKeyWord_t)&MaterialMap_t::SetMapName},
      {"*MAP_CLASS", (OnKeyWord_t)&MaterialMap_t::SetMapClass},
      {"*MAP_SUBNO", (OnKeyWord_t)&MaterialMap_t::SetMapSubno},
      {"*MAP_AMOUNT", (OnKeyWord_t)&MaterialMap_t::SetMapAmount},
      {"*BITMAP", (OnKeyWord_t)&MaterialMap_t::SetBitmap},
      {"*MAP_TYPE", (OnKeyWord_t)&MaterialMap_t::SetMapType},
      {"*UVW_U_OFFSET", (OnKeyWord_t)&MaterialMap_t::SetUvwUOffset},
      {"*UVW_V_OFFSET", (OnKeyWord_t)&MaterialMap_t::SetUvwVOffset},
      {"*UVW_U_TILING", (OnKeyWord_t)&MaterialMap_t::SetUvwUTiling},
      {"*UVW_V_TILING", (OnKeyWord_t)&MaterialMap_t::SetUvwVTiling},
      {"*UVW_ANGLE", (OnKeyWord_t)&MaterialMap_t::SetUvwAngle},
      {"*UVW_BLUR", (OnKeyWord_t)&MaterialMap_t::SetUvwBlur},
      {"*UVW_BLUR_OFFSET", (OnKeyWord_t)&MaterialMap_t::SetUvwBlurOffset},
      {"*UVW_NOUSE_AMT", (OnKeyWord_t)&MaterialMap_t::SetUvwNouseAmt},
      {"*UVW_NOISE_SIZE", (OnKeyWord_t)&MaterialMap_t::SetUvwNoiseSize},
      {"*UVW_NOISE_LEVEL", (OnKeyWord_t)&MaterialMap_t::SetUvwNoiseLevel},
      {"*UVW_NOISE_PHASE", (OnKeyWord_t)&MaterialMap_t::SetUvwNoisePhase},
      {"*BITMAP_FILTER", (OnKeyWord_t)&MaterialMap_t::SetBitmapFilter}
   };

protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };
public:
  MaterialMap_t(std::vector<ASEParser_t*> * pObjs, const std::string & maptypename):ASEParser_t(pObjs){ MapTypeName = maptypename; }

  std::string MapTypeName;
  std::string MapName;
  std::string MapClass;
  int MapSubno;
  float MapAmount;
  std::string Bitmap;
  std::string MapType;
  float UvwUOffset;
  float UvwVOffset;
  float UvwUTiling;
  float UvwVTiling;
  float UvwAngle;
  float UvwBlur;
  float UvwBlurOffset;
  float UvwNouseAmt;
  float UvwNoiseSize;
  int UvwNoiseLevel;
  float UvwNoisePhase;
  std::string BitmapFilter;
};

class Material_t : public ASEParser_t
{
private:
  pError_t SetMaterialName(std::vector<std::string> & args);
  pError_t SetMaterialClass(std::vector<std::string> & args);
  pError_t SetMaterialAmbient(std::vector<std::string> & args);
  pError_t SetMaterialDiffuse(std::vector<std::string> & args);
  pError_t SetMaterialSpecular(std::vector<std::string> & args);
  pError_t SetMaterialShine(std::vector<std::string> & args);
  pError_t SetMaterialShinestrength(std::vector<std::string> & args);
  pError_t SetMaterialTransparency(std::vector<std::string> & args);
  pError_t SetMaterialWiresize(std::vector<std::string> & args);
  pError_t SetMaterialShading(std::vector<std::string> & args);
  pError_t SetMaterialXpFalloff(std::vector<std::string> & args);
  pError_t SetMaterialSelfillum(std::vector<std::string> & args);
  pError_t SetMaterialFalloff(std::vector<std::string> & args);
  pError_t SetMaterialXpType(std::vector<std::string> & args);
  pError_t SetMapDiffuse(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MATERIAL_NAME", (OnKeyWord_t)&Material_t::SetMaterialName},
      {"*MATERIAL_CLASS", (OnKeyWord_t)&Material_t::SetMaterialClass},
      {"*MATERIAL_AMBIENT", (OnKeyWord_t)&Material_t::SetMaterialAmbient},
      {"*MATERIAL_DIFFUSE", (OnKeyWord_t)&Material_t::SetMaterialDiffuse},
      {"*MATERIAL_SPECULAR", (OnKeyWord_t)&Material_t::SetMaterialSpecular},
      {"*MATERIAL_SHINE", (OnKeyWord_t)&Material_t::SetMaterialShine},
      {"*MATERIAL_SHINESTRENGTH", (OnKeyWord_t)&Material_t::SetMaterialShinestrength},
      {"*MATERIAL_TRANSPARENCY", (OnKeyWord_t)&Material_t::SetMaterialTransparency},
      {"*MATERIAL_WIRESIZE", (OnKeyWord_t)&Material_t::SetMaterialWiresize},
      {"*MATERIAL_SHADING", (OnKeyWord_t)&Material_t::SetMaterialShading},
      {"*MATERIAL_XP_FALLOFF", (OnKeyWord_t)&Material_t::SetMaterialXpFalloff},
      {"*MATERIAL_SELFILLUM", (OnKeyWord_t)&Material_t::SetMaterialSelfillum},
      {"*MATERIAL_FALLOFF", (OnKeyWord_t)&Material_t::SetMaterialFalloff},
      {"*MATERIAL_XP_TYPE", (OnKeyWord_t)&Material_t::SetMaterialXpType},
      {"*MAP_DIFFUSE", (OnKeyWord_t)&Material_t::SetMapDiffuse}
   };

protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };
public:
  Material_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}
  std::string MaterialName;
  std::string MaterialClass;
  RGBf MaterialAmbient;
  RGBf MaterialDiffuse;
  RGBf MaterialSpecular;
  float MaterialShine;
  float MaterialShinestrength;
  float MaterialTransparency;
  float MaterialWiresize;
  std::string MaterialShading;
  float MaterialXpFalloff;
  float MaterialSelfillum;
  std::string MaterialFalloff;
  std::string MaterialXpType;
  std::vector<MaterialMap_t *> MaterialMaps;
};

class MaterialList_t: public ASEParser_t
{
private:
  pError_t SetMaterialCount(std::vector<std::string> & args);
  pError_t SetMaterial(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MATERIAL_COUNT", (OnKeyWord_t)&MaterialList_t::SetMaterialCount},
      {"*MATERIAL", (OnKeyWord_t)&MaterialList_t::SetMaterial}
   };

protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };
public:
  MaterialList_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}
  int MaterialCount;
  std::vector<Material_t *> Material;
};

class NodeTm_t : public ASEParser_t
{
private:
  pError_t SetNodeName(std::vector<std::string> & args);
  pError_t SetInheritPos(std::vector<std::string> & args);
  pError_t SetInheritRot(std::vector<std::string> & args);
  pError_t SetInheritScl(std::vector<std::string> & args);
  pError_t SetTmRow0(std::vector<std::string> & args);
  pError_t SetTmRow1(std::vector<std::string> & args);
  pError_t SetTmRow2(std::vector<std::string> & args);
  pError_t SetTmRow3(std::vector<std::string> & args);
  pError_t SetTmPos(std::vector<std::string> & args);
  pError_t SetTmRotaxis(std::vector<std::string> & args);
  pError_t SetTmRotangle(std::vector<std::string> & args);
  pError_t SetTmScale(std::vector<std::string> & args);
  pError_t SetTmScaleaxis(std::vector<std::string> & args);
  pError_t SetTmScaleaxisang(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*NODE_NAME", (OnKeyWord_t)&NodeTm_t::SetNodeName},
      {"*INHERIT_POS", (OnKeyWord_t)&NodeTm_t::SetInheritPos},
      {"*INHERIT_ROT", (OnKeyWord_t)&NodeTm_t::SetInheritRot},
      {"*INHERIT_SCL", (OnKeyWord_t)&NodeTm_t::SetInheritScl},
      {"*TM_ROW0", (OnKeyWord_t)&NodeTm_t::SetTmRow0},
      {"*TM_ROW1", (OnKeyWord_t)&NodeTm_t::SetTmRow1},
      {"*TM_ROW2", (OnKeyWord_t)&NodeTm_t::SetTmRow2},
      {"*TM_ROW3", (OnKeyWord_t)&NodeTm_t::SetTmRow3},
      {"*TM_POS", (OnKeyWord_t)&NodeTm_t::SetTmPos},
      {"*TM_ROTAXIS", (OnKeyWord_t)&NodeTm_t::SetTmRotaxis},
      {"*TM_ROTANGLE", (OnKeyWord_t)&NodeTm_t::SetTmRotangle},
      {"*TM_SCALE", (OnKeyWord_t)&NodeTm_t::SetTmScale},
      {"*TM_SCALEAXIS", (OnKeyWord_t)&NodeTm_t::SetTmScaleaxis},
      {"*TM_SCALEAXISANG", (OnKeyWord_t)&NodeTm_t::SetTmScaleaxisang},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  NodeTm_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::string NodeName;
  Int3 InheritPos;
  Int3 InheritRot;
  Int3 InheritScl;
  Float3 TmRow0;
  Float3 TmRow1;
  Float3 TmRow2;
  Float3 TmRow3;
  Vector3f TmPos;
  Vector3f TmRotaxis;
  float TmRotangle;
  Vector3f TmScale;
  Vector3f TmScaleaxis;
  float TmScaleaxisang;
};

class MeshVertexList_t: public ASEParser_t
{
private:
  pError_t SetMeshVertex(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MESH_VERTEX", (OnKeyWord_t)&MeshVertexList_t::SetMeshVertex},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  MeshVertexList_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::vector<Vector3f> MeshVertex;
};

class MeshFaceList_t: public ASEParser_t
{
private:
  pError_t SetMeshFace(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MESH_FACE", (OnKeyWord_t)&MeshFaceList_t::SetMeshFace},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  MeshFaceList_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::vector<MeshFace_t> MeshFace;
};

class MeshTVertList_t: public ASEParser_t
{
private:
  pError_t SetMeshTVert(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MESH_TVERT", (OnKeyWord_t)&MeshTVertList_t::SetMeshTVert},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  MeshTVertList_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::vector<Vector3f> MeshTVert;
};

class MeshTFaceList_t: public ASEParser_t
{
private:
  pError_t SetMeshTFace(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*MESH_TFACE", (OnKeyWord_t)&MeshTFaceList_t::SetMeshTFace},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  MeshTFaceList_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::vector<MeshTFace_t> MeshTFace;
};

class MeshNormals_t: public ASEParser_t
{
private:
  pError_t SetMeshFaceNormal(std::vector<std::string> & args);
  pError_t SetMeshVertexNormal(std::vector<std::string> & args);


  Lexems_t Lexs = {
      {"*MESH_FACENORMAL", (OnKeyWord_t)&MeshNormals_t::SetMeshFaceNormal},
      {"*MESH_VERTEXNORMAL", (OnKeyWord_t)&MeshNormals_t::SetMeshVertexNormal},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  MeshNormals_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::vector<Vector3f> MeshFaceNormal;

};

class Mesh_t: public ASEParser_t
{
private:
  pError_t SetTimevalue(std::vector<std::string> & args);
  pError_t SetMeshNumvertex(std::vector<std::string> & args);
  pError_t SetMeshNumfaces(std::vector<std::string> & args);
  pError_t SetMeshVertexList(std::vector<std::string> & args);
  pError_t SetMeshFaceList(std::vector<std::string> & args);
  pError_t SetMeshNumtvertex(std::vector<std::string> & args);
  pError_t SetMeshTVertList(std::vector<std::string> & args);
  pError_t SetMeshNumTVFaces(std::vector<std::string> & args);
  pError_t SetMeshTFaceList(std::vector<std::string> & args);
  pError_t SetMeshNumCVertex(std::vector<std::string> & args);
  pError_t SetMeshNormals(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*TIMEVALUE", (OnKeyWord_t)&Mesh_t::SetTimevalue},
      {"*MESH_NUMVERTEX", (OnKeyWord_t)&Mesh_t::SetMeshNumvertex},
      {"*MESH_NUMFACES", (OnKeyWord_t)&Mesh_t::SetMeshNumfaces},
      {"*MESH_VERTEX_LIST", (OnKeyWord_t)&Mesh_t::SetMeshVertexList},
      {"*MESH_FACE_LIST", (OnKeyWord_t)&Mesh_t::SetMeshFaceList},
      {"*MESH_NUMTVERTEX", (OnKeyWord_t)&Mesh_t::SetMeshNumtvertex},
      {"*MESH_TVERTLIST", (OnKeyWord_t)&Mesh_t::SetMeshTVertList},
      {"*MESH_NUMTVFACES", (OnKeyWord_t)&Mesh_t::SetMeshNumTVFaces},
      {"*MESH_TFACELIST", (OnKeyWord_t)&Mesh_t::SetMeshTFaceList},
      {"*MESH_NUMCVERTEX", (OnKeyWord_t)&Mesh_t::SetMeshNumCVertex},
      {"*MESH_NORMALS", (OnKeyWord_t)&Mesh_t::SetMeshNormals},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  Mesh_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  int Timevalue;
  int MeshNumvertex;
  int MeshNumfaces;
  MeshVertexList_t * MeshVertexList;
  MeshFaceList_t * MeshFaceList;
  int MeshNumtvertex;
  MeshTVertList_t * MeshTVertList;
  int MeshNumTVFaces;
  MeshTFaceList_t * MeshTFaceList;
  int MeshNumCVertex;
  MeshNormals_t * MeshNormals;
};

class Geomobject_t:public ASEParser_t
{
private:
  pError_t SetNodeName(std::vector<std::string> & args);
  pError_t SetNodeTm(std::vector<std::string> & args);
  pError_t SetMesh(std::vector<std::string> & args);
  pError_t SetPropMotionBlur(std::vector<std::string> & args);
  pError_t SetPropCastShadow(std::vector<std::string> & args);
  pError_t SetPropRecvShadow(std::vector<std::string> & args);
  pError_t SetMaterialRef(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*NODE_NAME", (OnKeyWord_t)&Geomobject_t::SetNodeName},
      {"*NODE_TM", (OnKeyWord_t)&Geomobject_t::SetNodeTm},
      {"*MESH", (OnKeyWord_t)&Geomobject_t::SetMesh},
      {"*PROP_MOTIONBLUR", (OnKeyWord_t)&Geomobject_t::SetPropMotionBlur},
      {"*PROP_CASTSHADOW", (OnKeyWord_t)&Geomobject_t::SetPropCastShadow},
      {"*PROP_RECVSHADOW", (OnKeyWord_t)&Geomobject_t::SetPropRecvShadow},
      {"*MATERIAL_REF", (OnKeyWord_t)&Geomobject_t::SetMaterialRef}
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  Geomobject_t(std::vector<ASEParser_t*> * pObjs):ASEParser_t(pObjs){}

  std::string NodeName;
  NodeTm_t * NodeTm;
  Mesh_t * Mesh;
  int PropMotionBlur;
  int PropCastShadow;
  int PropRecvShadow;
  int MaterialRef;
};

class LoaderASE_t: public ASEParser_t
{
private:
  pError_t ASCIIExport(std::vector<std::string> & args);
  pError_t SetSceneObj(std::vector<std::string> & args);
  pError_t SetMaterialList(std::vector<std::string> & args);
  pError_t SetGeomobject(std::vector<std::string> & args);

  Lexems_t Lexs = {
      {"*3DSMAX_ASCIIEXPORT", (OnKeyWord_t)&LoaderASE_t::ASCIIExport},
      {"*COMMENT", &LoaderASE_t::Skip},
      {"*SCENE", (OnKeyWord_t)&LoaderASE_t::SetSceneObj},
      {"*MATERIAL_LIST", (OnKeyWord_t)&LoaderASE_t::SetMaterialList},
      {"*GEOMOBJECT", (OnKeyWord_t)&LoaderASE_t::SetGeomobject},
  };
protected:
  virtual Lexems_t & Lexems(void){ return Lexs; };

public:
  LoaderASE_t();
  virtual ~LoaderASE_t();
  virtual pError_t LoadFile(const std::string filename);
  Scene_t * Scene;
  MaterialList_t * MaterialList;
  std::vector<Geomobject_t *> Geomobject;
};

#endif /* OBJ3D_H_ */
