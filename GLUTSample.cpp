/******************************************************************************\
* Copyright (C) 2012-2017 Ultraleap Ltd. All rights reserved.                  *
* Ultraleap proprietary and confidential. Not for distribution.                *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Ultraleap and you, your company or other organization.               *
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <time.h>
#include <LeapC.h>
#include "ExampleConnection.h"

// #define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

int32_t lastDrawnFrameId = 0;
volatile int32_t newestFrameId = 0;
int window; // GLUT window handle

/* Notifies us that a new frame is available. */
void OnFrame(const LEAP_TRACKING_EVENT *frame){
  newestFrameId = (int32_t)frame->tracking_frame_id;
}

static glm::quat toGlm(const LEAP_QUATERNION& q)
{
    return glm::quat(q.w, q.x, q.y, q.z);
}

static glm::vec3 toGlm(const LEAP_VECTOR& v)
{
    return glm::vec3(v.x, v.y, v.z);
}

static void glRotatef(const LEAP_QUATERNION& q)
{
  auto angle = glm::degrees(glm::angle(toGlm(q)));
  auto axis = glm::axis(toGlm(q));
  glRotatef(angle, axis.x, axis.y, axis.z);
}

static void glTranslatef(const LEAP_VECTOR& v)
{
  glTranslatef(v.x, v.y, v.z);
}

static float distance(const LEAP_VECTOR& p1, const LEAP_VECTOR& p2)
{
  return glm::distance(toGlm(p1), toGlm(p2));
}

void display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(0, -300, -500); //"Camera" viewpoint
  glClear(GL_COLOR_BUFFER_BIT);

  LEAP_TRACKING_EVENT *frame = GetFrame();
  if (frame) {

      for (uint32_t h = 0; h < frame->nHands; h++) {
          // Draw the hand
          LEAP_HAND* hand = &frame->pHands[h];
          //elbow
          glPushMatrix();
          glTranslatef(hand->arm.prev_joint.x, hand->arm.prev_joint.y, hand->arm.prev_joint.z);
          glutWireOctahedron();
          glPopMatrix();

          //wrist
          glPushMatrix();
          glTranslatef(hand->arm.next_joint.x, hand->arm.next_joint.y, hand->arm.next_joint.z);
          glutWireOctahedron();
          glPopMatrix();

          //palm position
          auto position = hand->palm.position;
          auto orientation = hand->palm.orientation;
          auto normal = hand->palm.normal;
          auto width = hand->palm.width;

          glPushMatrix();
          glTranslatef(position);
          glRotatef(orientation);
          glutWireCube(width);
          glPopMatrix();

          glBegin(GL_LINES);
          glVertex3f(position.x, position.y, position.z);
          auto end = toGlm(position) + toGlm(normal) * width;
          glVertex3f(end.x, end.y, end.z);
          glEnd();

          //Distal ends of bones for each digit
          for (int f = 0; f < 5; f++) {
              LEAP_DIGIT finger = hand->digits[f];
              for (int b = 0; b < 4; b++) {
                  LEAP_BONE bone = finger.bones[b];
                  glPushMatrix();
                  glTranslatef(bone.prev_joint);
                  glRotatef(bone.rotation);
                  auto begin = toGlm(bone.prev_joint);
                  auto end = toGlm(bone.next_joint);
                  glutSolidCylinder(bone.width/2.0, distance(begin, end), 10, 10);

                  //glTranslatef(bone.next_joint.x, bone.next_joint.y, bone.next_joint.z);
                  //glutWireOctahedron();
                  
                  glPopMatrix();
              }
          }
          // End of draw hand
      }

  }
  glFlush();
  glPopMatrix();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 640/240, 1.0, 1000);
}

void keyboard(unsigned char key, int x, int y)
{
  switch((char)key) {
  case 'q':
  case 27:  // ESC
    glutDestroyWindow(window);
    CloseConnection();
    exit(0);
  default:
    break;
  }
}

void idle(void){
  if(lastDrawnFrameId < newestFrameId){
    lastDrawnFrameId = newestFrameId;
    glutPostRedisplay();
  }
}

int main(int argc, char *argv[])
{
  ConnectionCallbacks.on_frame = OnFrame;
  OpenConnection();

  while(!IsConnected){
    millisleep(250);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(1280, 720);
  window = glutCreateWindow("LeapC Example");

  // GLUT callbacks
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);

  // init GL
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(3.0);

  // Start GLUT loop
  glutMainLoop();

  CloseConnection();
  return 0;
}
//End-of-Sample
