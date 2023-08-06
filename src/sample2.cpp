#include <glut.h>
#include <stdlib.h>

GLdouble vertex[][3] = {
  {0.0, 0.0, 0.0}, // A
  {1.0, 0.0, 0.0}, // B
  {1.0, 1.0, 0.0}, // C
  {0.0, 1.0, 0.0}, // D
  {0.0, 0.0, 1.0}, // E
  {1.0, 0.0, 1.0}, // F
  {1.0, 1.0, 1.0}, // G
  {0.0, 1.0, 1.0}  // H
};

int edge[][2] = {
  {0, 1}, // A -> B ア
  {1, 2}, // B -> C イ
  {2, 3}, // C -> D ウ
  {3, 0}, // D -> A エ
  {4, 5}, // E -> F オ
  {5, 6}, // F -> G カ
  {6, 7}, // G -> H キ
  {7, 4}, // H -> E ク
  {0, 4}, // A -> E ケ
  {1, 5}, // B -> F コ
  {2, 6}, // C -> G サ
  {3, 7}  // D -> H シ
};

void idle(void){
  glutPostRedisplay(); // 再描画イベントを発生させる.
}

void display(void){
  int i;
  static int r = 0; // 回転角
  glClear(GL_COLOR_BUFFER_BIT);//背景色で塗る
  glLoadIdentity();

  // 視点位置と視線方向
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // 図形の回転
  glRotated((double)r, 0.0, 1.0, 0.0);

  // 図形の描画
  glColor3d(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
  for(i = 0; i< 12; i++){
    glVertex3dv(vertex[edge[i][0]]);
    glVertex3dv(vertex[edge[i][1]]);
  }

  glEnd();

  glFlush();

  // 一周回ったら回転角を 0 に戻す
  if (r++ >= 360) r = 0;
  
}

void resize(int w, int h){
  glViewport(0, 0, w, h);

  // 透視変換行列の設定
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

  // モデルビュー変換行列の設定
  glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y){
  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN){
      // アニメーション開始
      glutIdleFunc(idle);
    }
    else{
      // アニメーション停止
      glutIdleFunc(0); // 0 でも NULL でも OK
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if(state == GLUT_DOWN){
      // コマ送り (1 ステップだけ進める)
      glutPostRedisplay();
    }
    break;
  default:
    break;
  }
}

void keyboard(unsigned char key, int x, int y){
  switch(key){
  case 'q':
  case 'Q':
  case '\033': // '\033' は ESC の ASCII のコード
    exit(0);
  default:
    break;
  }
}

void init(void){
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc ,char *argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}