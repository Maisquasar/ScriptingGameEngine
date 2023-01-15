import Math
import math
import S_Transform
import S_Camera
import S_Rigidbody
import S_Player

def GameUpdate(player, deltaTime:float):
	player.InputUpdate()
	# Apply Movement.

	dSpeed = deltaTime * player.Speed
	theta = Math.ToRadians(player._cam.rotation.x)
	delta = Math.Vec3D(dSpeed * player._input.right - dSpeed * player._input.left, 0, dSpeed * player._input.backward - dSpeed * player._input.forward)
	player._rb.Velocity = player._rb.Velocity + Math.Vec3D(math.cos(theta) * delta.x + math.sin(theta) * delta.z, delta.y, -math.sin(theta) * delta.x + math.cos(theta) * delta.z)
	player.GetTransform().SetWorldRotation(Math.Vec3D(0, player._cam.rotation.x - 180, 0))
	if (player._input.jump and player._rb.Grounded) :
 		player._rb.Velocity = player._rb.Velocity + (Math.Vec3D(0, player.JumpForce, 0))
