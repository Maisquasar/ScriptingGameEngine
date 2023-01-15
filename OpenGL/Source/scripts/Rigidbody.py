import Math
import S_Rigidbody
import S_Transform

def ApplyGravity(rb, dt:float):
	force = Math.Vec3D(0)
	force = force + Math.Vec3D(0, 1, 0) * rb.Mass * rb.Gravity # apply a force
	rb.Velocity =  rb.Velocity + force / rb.Mass * dt

def GameUpdate(rb, dt:float):
	if rb.IsInPause():
		return None
	if rb.enableGravity:
		ApplyGravity(rb, dt)

	if rb.Velocity.GetLength() > rb.Max:
		rb.Velocity = Math.Vec3D.ClampMagnitude(rb.Velocity, rb.Max);
	
	# Apply Velocity.
	rb.GetTransform().SetWorldPosition((rb.Velocity * dt) + rb.GetTransform().GetWorldPosition())
    