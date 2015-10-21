/* 
 Camera.h
 Quaternion camera code adapted from: http://hamelot.co.uk/visualization/opengl-camera/
 Based on code by Hammad Mazhar, https://github.com/hmazhar/moderngl_camera
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "../utils/glm/fwd.hpp"
#include "../utils/glm/vec3.hpp"
#include "../utils/glm/mat4x4.hpp"
#include "../utils/glm/gtc/quaternion.hpp"

/*class Camera
{
public:
    Camera();

    void lookAt( const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);
    void setPosition( const glm::vec3 &p ) { d.position =p; }
    glm::vec3 position() const { return d.position; }
    void setTarget( const glm::vec3 & );
    glm::vec3 target() const { return d.target; }
    void setUp( const glm::vec3 & );
    glm::vec3 up() const { return d.up; }
    void setAspect( float );
    float aspect() const { return d.aspect; }
    void perspective( float fov, float near, float far );
    void orbit( float yaw, float pitch );
    void translate( const glm::vec3 & );
    void setFov( float );

    float fov( ) const;

    glm::mat4 &projectionMatrix( );
    const float *pMatrix( ) const;

    glm::mat4 &viewMatrix( );
    const float *vMatrix( ) const;

    void update( );

private:

    class CameraPrivate
    {
    public:
        float vertAngle;
        float aspect;
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up;
        glm::quat rotation;
        float near;
        float far;
        glm::mat4 vmatrix;
        glm::mat4 pmatrix;
        float yaw, pitch, roll;

    } d;

};*/

class Camera {
	public:
		enum CameraDirection {
			UP, DOWN, LEFT, RIGHT, FORWARD, BACK
		};
		Camera( glm::vec3 position = glm::vec3(0,3,3), 
				glm::vec3 target =glm::vec3(0,0,0), 
				double fov =45.0);
		~Camera();

		void update();


		void moveLocal( CameraDirection dir, float distance );
		void rotate( float heading, float pitch );
		void orbit( float yaw, float pitch );

		void setPosition(glm::vec3 pos);
		glm::vec3 position() const { return camera_position; }

		void setTarget(glm::vec3 pos);
		glm::vec3 target() const { return camera_target; }

		void setFOV(double fov);
		double fov() const { return field_of_view; }

		void setClipping(double near_clip_distance, double far_clip_distance);
		double nearClipDistance() const { return near_clip; }
		double farClipDistance() const { return far_clip; }
		
		void setAspect( double a ) { camera_aspect = a; }
		double aspect() const { return camera_aspect; }

		glm::mat4 projectionMatrix() const { return projection; }
		glm::mat4 viewMatrix() const { return view; }
		

	private:
		double camera_aspect;
		double field_of_view;
		double near_clip;
		double far_clip;

		float camera_heading;
		float camera_pitch;
		glm::vec3 camera_position_delta;
		
		float orbit_yaw;
		float orbit_pitch;

		float max_pitch_rate;
		float max_heading_rate;

		glm::vec3 camera_position;
		glm::vec3 camera_target;
		glm::vec3 camera_direction;

		glm::vec3 camera_up;

		glm::mat4 projection;
		glm::mat4 view;
};

#endif // CAMERA_H
