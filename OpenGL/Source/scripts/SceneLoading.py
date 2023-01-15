import Math
import json
import S_Resource
import S_App
import S_Shader
import S_Texture
import S_Material
import S_Mesh
import S_Colliders
import S_CameraLock
import S_Rigidbody
import S_Lights
import S_Transform
import S_Player
import S_PauseMenu
import S_MainMenu
import S_RotationController
import Components
import GameObject

def LoadScene(node, app, Name):
    f = open(Name)
    data = json.load(f)
    LoadNode(node, data, app)

def LoadNode(node, data, app):
    node.Name = data["Name"]
    node.transform.SetLocalPosition(Math.Vec3D(data["Local Position"][0], data["Local Position"][1], data["Local Position"][2]))
    node.transform.SetLocalRotation(Math.Vec3D(data["Local Rotation"][0], data["Local Rotation"][1], data["Local Rotation"][2]))
    node.transform.SetLocalScale(Math.Vec3D(data["Local Scale"][0], data["Local Scale"][1], data["Local Scale"][2]))
    for i in data["Component"]:
        LoadComponent(node, i, app)
    for i in data["Children"]:
        child = GameObject.CreateNode()
        LoadNode(child, i, app)
        node.addChild(child)

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

def LoadMesh(node, data, app):
    mesh = S_Mesh.CreateMesh()
    if S_App.MULTITHREAD() is True:
        m = app.GetResourcesManager().GetMesh(data["Mesh Name"])
        if m is not None and m.IsLoaded() is True:
            mesh.Copy(m)
        else:
            app.AddToWaitlist(node, data["Mesh Name"])
    else:
        mesh.Copy(app.GetResourcesManager().GetMesh(data["Mesh Name"]))
    mesh.CurrentMaterial = LoadMaterial(data["Material"], app)
    mesh.CurrentTexture = LoadTexture(data["Texture"], app)
    mesh.SetShader(app.GetResourcesManager().GetShader(data["Shader"]))
    return mesh
#endregion
#region Colliders
def LoadSphCol(data):
    sph = S_Colliders.CreateSphereCollider()
    sph.Radius = data["Radius"]
    sph.Offset = Math.Vec3D(data["Center"][0], data["Center"][1], data["Center"][2])
    sph.Friction = Math.Vec3D(data["Friction"][0], data["Friction"][1], data["Friction"][2])
    sph.Static = data["Static"]
    return sph

def LoadBoxCol(data):
    box = S_Colliders.CreateBoxCollider()
    box.Size = Math.Vec3D(data["Size"][0], data["Size"][1], data["Size"][2])
    box.Offset = Math.Vec3D(data["Offset"][0], data["Offset"][1], data["Offset"][2])
    box.Friction = Math.Vec3D(data["Friction"][0], data["Friction"][1], data["Friction"][2])
    box.Static = data["Static"]
    return box

def LoadCapCol(data):
    cap = S_Colliders.CreateCapsuleCollider()
    cap.Radius = data["Radius"]
    cap.Height = data["Height"]
    cap.Offset = Math.Vec3D(data["Center"][0], data["Center"][1], data["Center"][2])
    cap.Friction = Math.Vec3D(data["Friction"][0], data["Friction"][1], data["Friction"][2])
    cap.Static = data["Static"]
    return cap
#endregion
#region CameraLock
def LoadCameraLock(data):
    return S_CameraLock.CreateCameraLock()
#endregion
#region PlayerScript
def LoadPlayerScript(data):
    ps = S_Player.CreatePlayerScript()
    ps.Speed = data["Speed"]
    ps.JumpForce = data["Jump"]
    return ps
#endregion
#region Physic
def LoadRigidbody(node, data):
    rb = S_Rigidbody.CreateRigidbody()
    rb.gameObject = node
    rb.Gravity = data["Gravity"]
    rb.Mass = data["Mass"]
    rb.Max = data["Max"]
    rb.Friction = Math.Vec3D(data["Friction"][0], data["Friction"][1], data["Friction"][2])
    rb.Initialize()
    return rb
#endregion
#region Lights
def LoadDirLight(data):
    dir = S_Lights.CreateDirLight()
    dir.Index = data["Index"]
    dir.Ambient = Math.Vec4D(data["Ambient"][0], data["Ambient"][1], data["Ambient"][2], 1)
    dir.Diffuse = Math.Vec4D(data["Diffuse"][0], data["Diffuse"][1], data["Diffuse"][2], 1)
    dir.Specular = Math.Vec4D(data["Specular"][0], data["Specular"][1], data["Specular"][2], 1)
    dir.Direction = Math.Vec3D(data["Direction"][0], data["Direction"][1], data["Direction"][2])
    dir.AmbientStrenght = data["Strenght"][0]
    dir.SpecularStrenght = data["Strenght"][1]
    return dir

def LoadPtLight(data):
    pt = S_Lights.CreatePtLight()
    pt.Index = data["Index"]
    pt.Ambient = Math.Vec4D(data["Ambient"][0], data["Ambient"][1], data["Ambient"][2], 1)
    pt.Diffuse = Math.Vec4D(data["Diffuse"][0], data["Diffuse"][1], data["Diffuse"][2], 1)
    pt.Specular = Math.Vec4D(data["Specular"][0], data["Specular"][1], data["Specular"][2], 1)
    pt.AmbientStrenght = data["Strenght"][0]
    pt.SpecularStrenght = data["Strenght"][1]
    pt.Constant = data["Parameters"][0]
    pt.Linear = data["Parameters"][1]
    pt.Quadratic = data["Parameters"][2]
    return pt


def LoadSptLight(data):
    spt = S_Lights.CreateSptLight()
    spt.Index = data["Index"]
    spt.Ambient = Math.Vec4D(data["Ambient"][0], data["Ambient"][1], data["Ambient"][2], 1)
    spt.Diffuse = Math.Vec4D(data["Diffuse"][0], data["Diffuse"][1], data["Diffuse"][2], 1)
    spt.Specular = Math.Vec4D(data["Specular"][0], data["Specular"][1], data["Specular"][2], 1)
    spt.Direction = Math.Vec3D(data["Direction"][0], data["Direction"][1], data["Direction"][2])
    spt.CutOff = data["Parameters"][0]
    spt.OuterCutOff = data["Parameters"][1]
    spt.Constant = data["Attenuation"][0]
    spt.Linear = data["Attenuation"][1]
    spt.Quadratic = data["Attenuation"][2]
    return spt
#endregion
#region Menu
def LoadPauseMenu(data):
    pm = S_PauseMenu.CreatePauseMenu()
    pm.SetScene(data["SceneName"])
    return pm

def LoadMainMenu(data):
    mm = S_MainMenu.CreateMainMenu()
    mm.SetScene(data["SceneName"])
    return mm
#endregion
#region Rotation Controller
def LoadRotationController(data):
    rc = S_RotationController.CreateRotationController()
    rc.Axis = Math.Int3D(data["Axis"][0], data["Axis"][1], data["Axis"][2])
    rc.Speed = Math.Vec3D(data["Speed"][0], data["Speed"][1], data["Speed"][2])
    return rc
#endregion
def LoadComponent(node, data, app):
    C = Components.CreateComponent()
    Type = data["Type"]
    if Type == 'Mesh' :
       C = LoadMesh(node, data, app)
    elif Type == 'Sphere Collider' :
       C = LoadSphCol(data)
    elif Type == 'Box Collider' :
       C = LoadBoxCol(data)
    elif Type == 'Capsule Collider' :
       C = LoadBoxCol(data)
    elif Type == 'Camera Lock' :
       C = LoadCameraLock(data)
    elif Type == 'PlayerScript' :
       C = LoadPlayerScript(data)
    elif Type == 'Rigidbody' :
       C = LoadRigidbody(node, data)
    elif Type == 'Directional Light' :
       C = LoadDirLight(data)
    elif Type == 'Point Light' :
       C = LoadPtLight(data)
    elif Type == 'SpotLight' :
       C = LoadSptLight(data)
    elif Type == 'PauseMenu' :
       C = LoadPauseMenu(data)
    elif Type == 'MainMenu' :
       C = LoadMainMenu(data)
    elif Type == 'Rotation Controller':
        C = LoadRotationController(data)
    node.AddComponent(C)