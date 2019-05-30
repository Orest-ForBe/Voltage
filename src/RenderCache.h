/*
 * RenderCache.h
 *
 *  Created on: Apr 8, 2019
 *      Author: tim
 */

#ifndef RENDERCACHE_H_
#define RENDERCACHE_H_

#include <map>
#include <vector>
#include <string>

class Obj3D
{
public:
  typedef struct
  {
    float X;
    float Y;
  } Pos2f;

  typedef struct
  {
    float X;
    float Y;
    float Z;
  } Pos3f;

  typedef struct
  {
    Pos3f N;
    Pos3f P[3];
    Pos2f T[3];
  } MeshInfo_t;

  typedef struct
  {
    int TextureID;
    std::vector<MeshInfo_t*> mesh;
  } Geom_t;
private:
  std::map<std::string, Geom_t*> Meshes;
public:
  Geom_t * AddMesh(const std::string Name);
  Geom_t * GetMesh(const std::string Name);
};


class RenderCache_t
{
private:
  std::map<std::string, Obj3D *> CacheObjects;
  std::map<std::string, unsigned int> CacheTextures;
public:
  RenderCache_t();
  virtual ~RenderCache_t();

  virtual int LoadObject(const std::string ObjectName, const std::string & filename);
  virtual Obj3D * Find(const std::string ObjectName);

  virtual unsigned int LoadTexture(const std::string TextureName);
  virtual unsigned int  FindTexture(const std::string TextureName);
};

#endif /* RENDERCACHE_H_ */
