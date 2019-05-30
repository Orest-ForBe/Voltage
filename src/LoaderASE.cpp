/*
 * LoaderASE_t.cpp
 *
 *  Created on: Apr 3, 2019
 *      Author: tim
 */

#include "LoaderASE.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <typeinfo>
#include <cxxabi.h>

#define LOG_ENABLE                  0

#if LOG_ENABLE
template<typename T>
std::ostream & operator<<(std::ostream & os, std::vector<T> vec)
{
  os << "{ ";
  if (vec.size() != 0)
  {
    os << "\"";
    std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(os, "\", \""));
    os << vec.back();
    os << "\" ";
  }
  os << "}";
  return os;
}
#endif

std::string capit(std::string str)
{
  size_t i;
  bool toc = true;
  if (str[0] == '*')
    str.erase(0, 1);

  for (i = 0; i < str.length(); i++)
  {
    if (toc && isalpha(str[i]))
    {
      str[i] = toupper(str[i]);
      toc = false;
    }
    else if (str[i] == '_')
      toc = true;
    else
      str[i] = tolower(str[i]);
  }
  for (i = 0; i < str.length(); i++)
    if (str[i] == '_')
      str.erase(i, 1);

  return str;
}


std::string& ASEParser_t::ltrim(std::string& str, const std::string& chars)
{
  str.erase(0, str.find_first_not_of(chars));
  return str;
}

std::string& ASEParser_t::rtrim(std::string& str, const std::string& chars)
{
  str.erase(str.find_last_not_of(chars) + 1);
  return str;
}

std::string& ASEParser_t::trim(std::string& str, const std::string& chars)
{
  return ltrim(rtrim(str, chars), chars);
}

std::vector<std::string> ASEParser_t::split(const std::string& str)
{
  int len = str.length();
  char qot;
  int toklen;
  std::vector<std::string> tokens;

  qot = 0x00;
  for (int i = 0; i < len; i++)
  {
    int start = i;
    if ((str[i] == '\"') || (str[i] == '\''))
      qot = str[i];

    if (qot)
    {
      i++;
      start++;
      while ((i < len) && (str[i] != qot))
        i++;
      if (i < len)
        qot = 0x00;
      toklen = i - start;
      i++;
    }
    else
    {
      while ((i < len) && (str[i] != ' '))
        i++;
      toklen = i - start;
    }
    tokens.push_back(str.substr(start, toklen));
    while ((i < (len - 1)) && (str[i + 1] == ' '))
      i++;
  }
  return tokens;
}

ASEParser_t::pError_t ASEParser_t::ProcessLine(std::ifstream & fs)
{
  std::string str;
  std::vector<std::string> tokens;
  OnKeyWord_t mthd;
  pError_t err = pUnknownToken;
  std::string makemacro;

  if (fs.eof())
  {
    err = pDone;
  }
  else
  {
    std::getline(fs, str);
    std::replace(str.begin(), str.end(), '\t', ' ');
    str = trim(str);
#if LOG_ENABLE
    for (size_t i = 0; i < Objs->size(); i++)
      std::cout << ">";
    std::cout << " " << str << std::endl;
#endif
    if (str.length() > 0)
    {
      tokens = split(str);
      makemacro = tokens[0];
      if (tokens[0] == "}")
      {
        Objs->pop_back();
        if (Objs->size() != 0)
          err = pOk;
        else
          err = pDone;
      }
      else
      {
        mthd = Objs->back()->Lexems()[tokens[0]];
        tokens.erase(tokens.begin());
        if (mthd)
          err = (Objs->back()->*mthd)(tokens);
        if (err != pOk)
        {
#if LOG_ENABLE
          std::cout << "Error parsing: " << str << std::endl << "Error code: " << err << std::endl << std::endl;
          std::cout << "{\"" << makemacro << "\", (OnKeyWord_t)&" << abi::__cxa_demangle(typeid(*Objs->back()).name(), 0, 0, 0) << "::Set" << capit(makemacro) << "}," << std::endl;

          std::cout << "pError_t Set" << capit(makemacro) << "(std::vector<std::string> & args);" << std::endl;
#endif
        }
      }
    }
    else
      err = pOk;
  }
  return err;
}

LoaderASE_t::LoaderASE_t():ASEParser_t(new std::vector<ASEParser_t*>)
{
}

ASEParser_t::pError_t LoaderASE_t::ASCIIExport(std::vector<std::string>& args)
{
  return pOk;
}

ASEParser_t::pError_t LoaderASE_t::SetSceneObj(std::vector<std::string>& args)
{
  Scene = new Scene_t(Objs);
  Objs->push_back(Scene);
  return pOk;
}

ASEParser_t::pError_t LoaderASE_t::SetMaterialList(std::vector<std::string>& args)
{
  MaterialList = new MaterialList_t(Objs);
  Objs->push_back(MaterialList);
  return pOk;
}

ASEParser_t::pError_t LoaderASE_t::SetGeomobject(std::vector<std::string>& args)
{
  Geomobject_t * gobj = new Geomobject_t(Objs);
  Objs->push_back(gobj);
  Geomobject.push_back(gobj);
  return pOk;
}

LoaderASE_t::~LoaderASE_t()
{
}

ASEParser_t::pError_t LoaderASE_t::LoadFile(const std::string filename)
{
  std::ifstream fs(filename);
  pError_t err;
  Objs->push_back(this);

  while ((err = ProcessLine(fs)) == pOk)
    ;

  fs.close();
#if LOG_ENABLE
  std::cout << "Load ASE error: " << err << std::endl;
#endif
  return err;
}

ASEParser_t::pError_t Scene_t::SetFileName(std::vector<std::string>& args)
{
  FileName = args[0];
  return pOk;
}

ASEParser_t::pError_t Scene_t::SetSceneFirstFrame(std::vector<std::string>& args)
{
  SceneFirstFrame = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Scene_t::SetSceneLastFrame(std::vector<std::string>& args)
{
  SceneLastFrame = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Scene_t::SetSceneFrameSpeed(std::vector<std::string>& args)
{
  SceneFrameSpeed = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Scene_t::SetSceneTicksPerFrame(std::vector<std::string>& args)
{
  SceneTicksPerFrame = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Scene_t::SetSceneBackgroundStatic(std::vector<std::string>& args)
{
  SceneBackgroundStatic.R = std::stof(args[0]);
  SceneBackgroundStatic.G = std::stof(args[1]);
  SceneBackgroundStatic.B = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t Scene_t::SetSceneAmbientStatic(std::vector<std::string>& args)
{
  SceneAmbientStatic.R = std::stof(args[0]);
  SceneAmbientStatic.G = std::stof(args[1]);
  SceneAmbientStatic.B = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t MaterialList_t::SetMaterialCount(std::vector<std::string>& args)
{
  MaterialCount = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialList_t::SetMaterial(std::vector<std::string>& args)
{
  Material.push_back(new Material_t(Objs));
  Objs->push_back(Material.back());

  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialName(std::vector<std::string>& args)
{
  MaterialName = args[0];
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialClass(std::vector<std::string>& args)
{
  MaterialClass = args[0];
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialAmbient(std::vector<std::string>& args)
{
  MaterialAmbient.R = std::stof(args[0]);
  MaterialAmbient.G = std::stof(args[1]);
  MaterialAmbient.B = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialDiffuse(std::vector<std::string>& args)
{
  MaterialDiffuse.R = std::stof(args[0]);
  MaterialDiffuse.G = std::stof(args[1]);
  MaterialDiffuse.B = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialSpecular(std::vector<std::string>& args)
{
  MaterialSpecular.R = std::stof(args[0]);
  MaterialSpecular.G = std::stof(args[1]);
  MaterialSpecular.B = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialShine(std::vector<std::string>& args)
{
  MaterialShine = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialShinestrength(std::vector<std::string>& args)
{
  MaterialShinestrength = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialTransparency(std::vector<std::string>& args)
{
  MaterialTransparency = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialWiresize(std::vector<std::string>& args)
{
  MaterialWiresize = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialShading(std::vector<std::string>& args)
{
  MaterialShading = args[0];
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialXpFalloff(std::vector<std::string>& args)
{
  MaterialXpFalloff = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialSelfillum(std::vector<std::string>& args)
{
  MaterialSelfillum = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialFalloff(std::vector<std::string>& args)
{
  MaterialFalloff = args[0];
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMaterialXpType(std::vector<std::string>& args)
{
  MaterialXpType = args[0];
  return pOk;
}

ASEParser_t::pError_t Material_t::SetMapDiffuse(std::vector<std::string>& args)
{
  MaterialMap_t * Map = new MaterialMap_t(Objs, "Diffuse");
  MaterialMaps.push_back(Map);
  Objs->push_back(Map);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetMapName(std::vector<std::string>& args)
{
  MapName = args[0];
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetMapClass(std::vector<std::string>& args)
{
  MapClass = args[0];
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetMapSubno(std::vector<std::string>& args)
{
  MapSubno = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetMapAmount(std::vector<std::string>& args)
{
  MapAmount = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetBitmap(std::vector<std::string>& args)
{
  Bitmap = args[0];
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetMapType(std::vector<std::string>& args)
{
  MapType = args[0];
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwUOffset(std::vector<std::string>& args)
{
  UvwUOffset = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwVOffset(std::vector<std::string>& args)
{
  UvwVOffset = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwUTiling(std::vector<std::string>& args)
{
  UvwUTiling = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwVTiling(std::vector<std::string>& args)
{
  UvwVTiling = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwAngle(std::vector<std::string>& args)
{
  UvwAngle = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwBlur(std::vector<std::string>& args)
{
  UvwBlur = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwBlurOffset(std::vector<std::string>& args)
{
  UvwBlurOffset = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwNouseAmt(std::vector<std::string>& args)
{
  UvwNouseAmt = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwNoiseSize(std::vector<std::string>& args)
{
  UvwNoiseSize = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwNoiseLevel(std::vector<std::string>& args)
{
  UvwNoiseLevel = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetUvwNoisePhase(std::vector<std::string>& args)
{
  UvwNoisePhase = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t MaterialMap_t::SetBitmapFilter(std::vector<std::string>& args)
{
  BitmapFilter = args[0];
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetNodeName(std::vector<std::string>& args)
{
  NodeName = args[0];
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetNodeTm(std::vector<std::string>& args)
{
  NodeTm = new NodeTm_t(Objs);
  Objs->push_back(NodeTm);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetNodeName(std::vector<std::string>& args)
{
  NodeName = args[0];
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetInheritPos(std::vector<std::string>& args)
{
  InheritPos.I1 = std::stoi(args[0]);
  InheritPos.I2 = std::stoi(args[1]);
  InheritPos.I3 = std::stoi(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetInheritRot(std::vector<std::string>& args)
{
  InheritRot.I1 = std::stoi(args[0]);
  InheritRot.I2 = std::stoi(args[1]);
  InheritRot.I3 = std::stoi(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetInheritScl(std::vector<std::string>& args)
{
  InheritScl.I1 = std::stoi(args[0]);
  InheritScl.I2 = std::stoi(args[1]);
  InheritScl.I3 = std::stoi(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmRow0(std::vector<std::string>& args)
{
  TmRow0.f1 = std::stof(args[0]);
  TmRow0.f2 = std::stof(args[1]);
  TmRow0.f3 = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmRow1(std::vector<std::string>& args)
{
  TmRow1.f1 = std::stof(args[0]);
  TmRow1.f2 = std::stof(args[1]);
  TmRow1.f3 = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmRow2(std::vector<std::string>& args)
{
  TmRow2.f1 = std::stof(args[0]);
  TmRow2.f2 = std::stof(args[1]);
  TmRow2.f3 = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmRow3(std::vector<std::string>& args)
{
  TmRow3.f1 = std::stof(args[0]);
  TmRow3.f2 = std::stof(args[1]);
  TmRow3.f3 = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmPos(std::vector<std::string>& args)
{
  TmPos.X = std::stof(args[0]);
  TmPos.Y = std::stof(args[1]);
  TmPos.Z = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmRotaxis(std::vector<std::string>& args)
{
  TmRotaxis.X = std::stof(args[0]);
  TmRotaxis.Y = std::stof(args[1]);
  TmRotaxis.Z = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmRotangle(std::vector<std::string>& args)
{
  TmRotangle = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmScale(std::vector<std::string>& args)
{
  TmScale.X = std::stof(args[0]);
  TmScale.Y = std::stof(args[1]);
  TmScale.Z = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmScaleaxis(std::vector<std::string>& args)
{
  TmScaleaxis.X = std::stof(args[0]);
  TmScaleaxis.Y = std::stof(args[1]);
  TmScaleaxis.Z = std::stof(args[2]);
  return pOk;
}

ASEParser_t::pError_t NodeTm_t::SetTmScaleaxisang(std::vector<std::string>& args)
{
  TmScaleaxisang = std::stof(args[0]);
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetMesh(std::vector<std::string>& args)
{
  Mesh = new Mesh_t(Objs);
  Objs->push_back(Mesh);
  return pOk;
}

ASEParser_t::pError_t MeshVertexList_t::SetMeshVertex(std::vector<std::string>& args)
{
  Vector3f val;
  val.X = std::stof(args[1]);
  val.Y = std::stof(args[3]);
  val.Z = std::stof(args[2]);
  MeshVertex.push_back(val);
  return pOk;
}

ASEParser_t::pError_t MeshFaceList_t::SetMeshFace(std::vector<std::string>& args)
{
  MeshFace_t mf;
  mf.A = std::stoi(args[2]);
  mf.B = std::stoi(args[4]);
  mf.C = std::stoi(args[6]);
  mf.SmoothGroup = 0;
  mf.MtlId = std::stoi(args[args.size() - 1]);
  MeshFace.push_back(mf);
  return pOk;
}

ASEParser_t::pError_t MeshTVertList_t::SetMeshTVert(std::vector<std::string>& args)
{
  Vector3f val;
  val.X = std::stof(args[1]);
  val.Y = std::stof(args[2]);
  val.Z = std::stof(args[3]);
  MeshTVert.push_back(val);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetTimevalue(std::vector<std::string>& args)
{
  Timevalue = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshNumvertex(std::vector<std::string>& args)
{
  MeshNumvertex = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshNumfaces(std::vector<std::string>& args)
{
  MeshNumfaces = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshVertexList(std::vector<std::string>& args)
{
  MeshVertexList = new MeshVertexList_t(Objs);
  Objs->push_back(MeshVertexList);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshFaceList(std::vector<std::string>& args)
{
  MeshFaceList = new MeshFaceList_t(Objs);
  Objs->push_back(MeshFaceList);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshNumtvertex(std::vector<std::string>& args)
{
  MeshNumvertex = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshTVertList(std::vector<std::string>& args)
{
  MeshTVertList = new MeshTVertList_t(Objs);
  Objs->push_back(MeshTVertList);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshNumTVFaces(std::vector<std::string>& args)
{
  MeshNumTVFaces = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshTFaceList(std::vector<std::string>& args)
{
  MeshTFaceList = new MeshTFaceList_t(Objs);
  Objs->push_back(MeshTFaceList);
  return pOk;
}

ASEParser_t::pError_t MeshTFaceList_t::SetMeshTFace(std::vector<std::string>& args)
{
  MeshTFace_t val;
  val.A = std::stof(args[1]);
  val.B = std::stof(args[2]);
  val.C = std::stof(args[3]);
  MeshTFace.push_back(val);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshNumCVertex(std::vector<std::string>& args)
{
  MeshNumCVertex = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Mesh_t::SetMeshNormals(std::vector<std::string>& args)
{
  MeshNormals = new MeshNormals_t(Objs);
  Objs->push_back(MeshNormals);
  return pOk;
}

ASEParser_t::pError_t MeshNormals_t::SetMeshFaceNormal(std::vector<std::string>& args)
{
  Vector3f val;
  val.X = std::stof(args[1]);
  val.Y = std::stof(args[2]);
  val.Z = std::stof(args[3]);
  MeshFaceNormal.push_back(val);
  return pOk;
}

ASEParser_t::pError_t MeshNormals_t::SetMeshVertexNormal(std::vector<std::string>& args)
{
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetPropMotionBlur(std::vector<std::string>& args)
{
  PropMotionBlur = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetPropCastShadow(std::vector<std::string>& args)
{
  PropCastShadow = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetPropRecvShadow(std::vector<std::string>& args)
{
  PropRecvShadow = std::stoi(args[0]);
  return pOk;
}

ASEParser_t::pError_t Geomobject_t::SetMaterialRef(std::vector<std::string>& args)
{
  MaterialRef = std::stoi(args[0]);
  return pOk;
}

