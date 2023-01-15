import Math
import math
import S_Camera
import S_Input
import Components
import S_CameraLock
import S_Transform

def FirstUpdate(cam, p_deltaTime:float, play:bool):
		# Update distance.
		cam.distance = Math.Cut(cam.distance - cam.input.Scroll / 3, 0.001, 100.0)
		# Update rotation.
		cam.rotation = cam.rotation  + Math.Vec2D(-cam.input.DeltaX / 10, cam.input.DeltaY / 10)
		cam.rotation = Math.Vec2D(Math.Mod(cam.rotation.x, 360.0), Math.Cut(cam.rotation.y, -89.99, 89.99))
		# Update focus
		dSpeed:float = p_deltaTime * cam.speed * (5.0 if cam.input.Shift else 1.0)
		delta = Math.Vec3D(dSpeed * cam.input.Right - dSpeed * cam.input.Left, dSpeed * cam.input.Up - dSpeed * cam.input.Down, dSpeed * cam.input.Backward - dSpeed * cam.input.Forward) / 20
		cam.focus = cam.focus + Math.Vec3D(math.cos(Math.ToRadians(cam.rotation.x)) * delta.x + math.sin(Math.ToRadians(cam.rotation.x)) * delta.z, delta.y, -math.sin(Math.ToRadians(cam.rotation.x)) * delta.x + math.cos(Math.ToRadians(cam.rotation.x)) * delta.z)
		# Update Position.
		cam.position = cam.focus + Math.Vec3D(math.sin(Math.ToRadians(cam.rotation.x)) * math.cos(Math.ToRadians(cam.rotation.y)), math.sin(Math.ToRadians(cam.rotation.y)), math.cos(Math.ToRadians(cam.rotation.x)) * math.cos(Math.ToRadians(cam.rotation.y))) * (0.001 if cam.distance == 0.0 else cam.distance)

		if (cam.Lockposition != None and cam.Lockposition.windowOpen and cam.Lockposition.enable and play):
			cam.focus = cam.Lockposition.GetTransform().GetWorldPosition()


def Update(cam, p_deltaTime:float, play:bool) :
	# Update Inputs
	cam.input.Update()
	# If Mouse captured and Is not in Play or is in play.
	if cam.input.MouseCaptured is True and play is True or play is False:
			cam.input.CameraUpdate();
	if cam.input.MouseCaptured is True and play is not True or play is True:
		if cam.input.MouseCaptured is False :
			cam.input.DeltaX = 0;
			cam.input.DeltaY = 0;
		FirstUpdate(cam, p_deltaTime, play)