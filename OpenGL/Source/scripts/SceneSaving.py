import Math
import json
import S_Resource
import S_App
import S_Shader
import S_Texture
import S_Material
import S_Mesh
import S_Transform
import Components
import GameObject

def SaveScene(node, app, Name):
    data = '{\n'
    space = ''
    data = SaveNode(node, data, space, app)
    data += '}'
    print(data)
    with open(Name, "w") as outfile:
       outfile.write(data)

def SaveNode(node, data, space, app):
    space += '\t';
    data += space + "\"Name\" : \"{}\", \n".format(node.Name);
    data += space +"\"Local Position\" : [{}, {}, {}],\n".format(node.transform.GetLocalPosition()[0], node.transform.GetLocalPosition()[1], node.transform.GetLocalPosition()[2])
    data += space +"\"Local Rotation\" : [{}, {}, {}],\n".format(node.transform.GetLocalRotation()[0], node.transform.GetLocalRotation()[1], node.transform.GetLocalRotation()[2])
    data += space +"\"Local Scale\" : [{}, {}, {}],\n".format(node.transform.GetLocalScale()[0], node.transform.GetLocalScale()[1], node.transform.GetLocalScale()[2])
    return data

"""
def LoadComponent(node, data, app):
    Comp = Components.CreateComponent()
    Type = data["Type"]
    print(Type)
    if (Type == 'Mesh'):
       C = LoadMesh(data, app)
    node.AddComponent(C)

#region Mesh

def LoadMaterial(data, app):
    mat = S_Material.Material()
    mat.Ambient = Math.Vec4D(data["Ambient"][0], data["Ambient"][1], data["Ambient"][2], 1)
    mat.Diffuse = Math.Vec4D(data["Diffuse"][0], data["Diffuse"][1], data["Diffuse"][2], 1)
    mat.Specular = Math.Vec4D(data["Specular"][0], data["Specular"][1], data["Specular"][2], 1)
    mat.Shininess = data["Shininess"]
    mat.SetNormalMap(app.GetResourcesManager().GetTexture(data["Normal"]))
    return mat

def LoadTexture(data, app):
    tex = app.GetResourcesManager().GetTexture(data["Name"])
    tex.Wrapping = data["Wrapping"]
    tex.Filter = data["Filter"]
    return tex

def LoadMesh(data, app):
    mesh = S_Mesh.CreateMesh()
    mesh.Copy(app.GetResourcesManager().GetMesh(data["Mesh Name"]))
    mesh.CurrentMaterial = LoadMaterial(data["Material"], app)
    mesh.CurrentTexture = LoadTexture(data["Texture"], app)
    mesh.SetShader(app.GetResourcesManager().GetShader(data["Shader"]))
    return mesh
#endregion
"""