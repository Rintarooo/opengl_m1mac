// https://ie.u-ryukyu.ac.jp/~e085739/c.opengl.kumamoto.html
#include <glut.h> // GLUTライブラリを使用

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);//背景色で塗る
  
  glBegin(GL_POLYGON); // 図形のタイプを指定する

  glColor3d(1.0, 0.0, 0.0); // 赤
  glVertex2d(-0.5, -0.5); // 第 3 象限

  glColor3d(0.0, 1.0, 0.0); // 緑
  glVertex2d(0.5, -0.5); // 第 4 象限
    
  glColor3d(0.0, 0.0, 1.0); // 青
  glVertex2d(0.5, 0.5); // 第 1 象限

  glColor3d(1.0, 1.0, 0.0); // 黄
  glVertex2d(-0.5, 0.5); // 第 2 象限
  
  glEnd();
  glFlush();
  
}

// ウインドウサイズ変更コールバックには， 新しいウインドウの幅と高さ(ピクセル単位)が渡されます
// ウインドウの縦横の比が変われば，図形がそれだけ歪んだ形で表示されることになります． 
// 描画ウインドウのサイズが変更されたときも図形の形状を保つには，クリッピングウインドウを描画ウインドウに合わせて変更する必要があります
void resize(int w, int h){
  glViewport(0, 0, w, h); // ウィンドウ全体をビューポートにする
  glLoadIdentity(); // 変換行列の初期化
  // スクリーン上の表示域をビューポートの大きさに比例させる
  glOrtho(-w / 400.0, w / 400.0, -h / 400.0, h /400.0, -1.0, 1.0);
}

void init(void){
  glClearColor(1.0, 1.0, 1.0, 1.0);// 背景色指定(白)
}

int main(int argc, char *argv[]){
  glutInit(&argc, argv); // GLUT 及び OpenGL 環境を初期化する
  glutCreateWindow(argv[1]); // ウィンドウを開く
  glutDisplayFunc(display); // display ウィンドウに描画する関数へのポインタ
// OpenGLでは，ウインドウのサイズの変更に対応するコールバックを用意できます． 登録にはGLUT.ReshapeFuncを使います． ウインドウサイズ変更コールバックには， 新しいウインドウの幅と高さ(ピクセル単位)が渡されます． なお，このコールバックは，ウインドウが最初に生成されるときにも呼び出されます．
  glutReshapeFunc(resize);
  init();
  glutMainLoop(); // 無限ループで, プログラムはイベントの待ち受け状態になる
  return 0;
}