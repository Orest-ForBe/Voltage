/*
 * RenderCache.cpp
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#include "RenderCache.h"
#include "LoaderASE.h"
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>

RenderCache_t::RenderCache_t()
{
  // TODO Auto-generated constructor stub

}

RenderCache_t::~RenderCache_t()
{
  // TODO Auto-generated destructor stub
}

Obj3D* RenderCache_t::Find(const std::string ObjectName)
{
  return CacheObjects[ObjectName];
}

int RenderCache_t::LoadObject(const std::string ObjectName, const std::string& filename)
{
  LoaderASE_t * iaseobj;
  Obj3D * iobj;
  Obj3D::Geom_t* mesh;
  Obj3D::MeshInfo_t * mi;
  Obj3D::Pos3f u, v;
  float len;
  if (CacheObjects.find(ObjectName) != CacheObjects.end())
    return 0; // Allready loaded
  iaseobj = new LoaderASE_t();
  if (iaseobj->LoadFile(filename) == ASEParser_t::pDone)
  {
    iobj = new Obj3D();
    for (size_t n = 0; n < iaseobj->Geomobject.size(); n++)
    {
      mesh = iobj->AddMesh(iaseobj->Geomobject[n]->NodeName);

      mesh->TextureID = LoadTexture(iaseobj->MaterialList->Material[iaseobj->Geomobject[n]->MaterialRef]->MaterialMaps[0]->Bitmap);
      for (size_t i = 0; i < iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace.size(); i++)
      {
        mi = new Obj3D::MeshInfo_t;
        mi->P[0].X = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].A].X;
        mi->P[0].Y = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].A].Y;
        mi->P[0].Z = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].A].Z;
        mi->P[1].X = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].B].X;
        mi->P[1].Y = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].B].Y;
        mi->P[1].Z = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].B].Z;
        mi->P[2].X = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].C].X;
        mi->P[2].Y = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].C].Y;
        mi->P[2].Z = iaseobj->Geomobject[n]->Mesh->MeshVertexList->MeshVertex[iaseobj->Geomobject[n]->Mesh->MeshFaceList->MeshFace[i].C].Z;

        mi->T[0].X = iaseobj->Geomobject[n]->Mesh->MeshTVertList->MeshTVert[iaseobj->Geomobject[n]->Mesh->MeshTFaceList->MeshTFace[i].A].X;
        mi->T[0].Y = 1 - iaseobj->Geomobject[n]->Mesh->MeshTVertList->MeshTVert[iaseobj->Geomobject[n]->Mesh->MeshTFaceList->MeshTFace[i].A].Y;
        mi->T[1].X = iaseobj->Geomobject[n]->Mesh->MeshTVertList->MeshTVert[iaseobj->Geomobject[n]->Mesh->MeshTFaceList->MeshTFace[i].B].X;
        mi->T[1].Y = 1 - iaseobj->Geomobject[n]->Mesh->MeshTVertList->MeshTVert[iaseobj->Geomobject[n]->Mesh->MeshTFaceList->MeshTFace[i].B].Y;
        mi->T[2].X = iaseobj->Geomobject[n]->Mesh->MeshTVertList->MeshTVert[iaseobj->Geomobject[n]->Mesh->MeshTFaceList->MeshTFace[i].C].X;
        mi->T[2].Y = 1 - iaseobj->Geomobject[n]->Mesh->MeshTVertList->MeshTVert[iaseobj->Geomobject[n]->Mesh->MeshTFaceList->MeshTFace[i].C].Y;

        u.X = mi->P[1].X - mi->P[0].X;
        u.Y = mi->P[1].Y - mi->P[0].Y;
        u.Z = mi->P[1].Z - mi->P[0].Z;
        v.X = mi->P[2].X - mi->P[0].X;
        v.Y = mi->P[2].Y - mi->P[0].Y;
        v.Z = mi->P[2].Z - mi->P[0].Z;

        mi->N.X = u.Y * v.Z - u.Z * v.Y;
        mi->N.Y = u.Z * v.X - u.X * v.Z;
        mi->N.Z = u.X * v.Y - u.Y * v.X;
        len = sqrt(mi->N.X * mi->N.X + mi->N.Y * mi->N.Y + mi->N.Z * mi->N.Z);
        if (len)
        {
          mi->N.X /= len;
          mi->N.Y /= len;
          mi->N.Z /= len;
        }
        mesh->mesh.push_back(mi);
      }
    }
    CacheObjects[ObjectName] = iobj;
  }
  delete iaseobj;
  return 0;
}

Obj3D::Geom_t* Obj3D::AddMesh(const std::string Name)
{
  Obj3D::Geom_t * mesh = new Obj3D::Geom_t;
  Meshes[Name] = mesh;
  return mesh;
}

Obj3D::Geom_t* Obj3D::GetMesh(const std::string Name)
{
  return Meshes[Name];
}

unsigned int RenderCache_t::LoadTexture(const std::string TextureName)
{
  SDL_Surface *TextureImage[1];
  std::string lFile;

  if (CacheTextures.find(TextureName) != CacheTextures.end())
    return FindTexture(TextureName);

  lFile = "models/" + TextureName.substr(TextureName.find_last_of("/\\") + 1);


  if (!(TextureImage[0] = IMG_Load(lFile.c_str())))
  {
    std::string err = "Error loading texture \"";
    err += TextureName;
    err += "\", ";
    err += SDL_GetError();
    throw err;
  }

//  image.flipVertically();

  unsigned int texture = 0;
//  glGenTextures(1, &texture);
//  glBindTexture(GL_TEXTURE_2D, texture);
////  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, /*image.getPixelsPtr()*/);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenTextures(1, &texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureImage[0]->w, TextureImage[0]->h, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureImage[0]->w, TextureImage[0]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

//  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TextureImage[0]->w, TextureImage[0]->h, GL_RGBA, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
//  glGetError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  SDL_FreeSurface(TextureImage[0]);

  CacheTextures[TextureName] = texture;

  return texture;

}

unsigned int RenderCache_t::FindTexture(const std::string TextureName)
{
  return CacheTextures[TextureName];
}
