#include "pch.h"
#include "PlyParser.h"
#include "External/rply/rply.h"

int globalVertexIndex;
int globalFaceIndex;
int globalFacePolygonIndex;
PlyTriangleMesh* globalMesh;
int VertexCallbackX(p_ply_argument argument)
{
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    double argValue = ply_get_argument_value(argument);
    globalMesh->vertices[globalVertexIndex][0] = argValue;
    return 1;
}

int VertexCallbackY(p_ply_argument argument)
{
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    double argValue = ply_get_argument_value(argument);
    globalMesh->vertices[globalVertexIndex][1] = argValue;
    return 1;
}

int VertexCallbackZ(p_ply_argument argument)
{
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    double argValue = ply_get_argument_value(argument);
    globalMesh->vertices[globalVertexIndex][2] = argValue;
    globalVertexIndex++;
    return 1;
}

int FaceCallback(p_ply_argument argument)
{
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    int argValue = static_cast<int>(ply_get_argument_value(argument));
    //looking for new polygon
    if (globalFacePolygonIndex == -1)
    {
        if (argValue == 3)
        {
            globalFacePolygonIndex = 2;
        }
        else
        {
            //err, can't handle non-triangles
            return 0;
        }
    }
    else
    {
        globalFacePolygonIndex--;
        globalMesh->indices[globalFaceIndex++] = argValue;
    }
    return 1;
}


bool ParseFromPly(const std::string& plyFilePath, PlyTriangleMesh& mesh)
{
    globalVertexIndex = 0;
    globalFaceIndex = 0;
    globalFacePolygonIndex = -1;
    globalMesh = &mesh;
    int nvertices, ntriangles;
    p_ply ply = ply_open(plyFilePath.c_str(), NULL, 0, NULL);
    if (!ply) return false;
    if (!ply_read_header(ply)) return false;
    nvertices = ply_set_read_cb(ply, "vertex", "x", VertexCallbackX, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", VertexCallbackY, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", VertexCallbackZ, NULL, 1);
    ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", FaceCallback, NULL, 0);
    mesh.vertices.resize(nvertices);
    mesh.indices.resize(ntriangles * 3);
    if (!ply_read(ply)) return false;
    ply_close(ply);
    return true;
}