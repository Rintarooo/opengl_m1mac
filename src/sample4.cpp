// https://www.natural-science.or.jp/article/20091202002143.php
#include <glut.h> // GLUTライブラリを使用


//----------------------------------------------------
// 変数の宣言
//----------------------------------------------------
int WindowPositionX = 200;  //生成するウィンドウ位置のX座標
int WindowPositionY = 200;  //生成するウィンドウ位置のY座標
int WindowWidth = 512;      //生成するウィンドウの幅
int WindowHeight = 512;     //生成するウィンドウの高さ
char WindowTitle[] = "世界の始まり";  //ウィンドウのタイトル

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void Initialize(void);   //初期設定時に呼び出す関数
void Idle(void);         //アイドル時に呼び出す関数
void Display(void);      //画面描画時に呼び出す関数
void Ground(void);       //大地の描画用の関数
//----------------------------------------------------
// メイン関数
//----------------------------------------------------
int main(int argc, char *argv[]){
  glutInit(&argc, argv);                                     //環境の初期化
  glutInitWindowPosition(WindowPositionX, WindowPositionY);  //ウィンドウの位置の指定
  glutInitWindowSize(WindowWidth, WindowHeight);             //ウィンドウサイズの指定
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //ディスプレイモードの指定
  glutCreateWindow(WindowTitle);                             //ウィンドウの作成
  glutIdleFunc(Idle);                                        //プログラムアイドル状態時に呼び出される関数
  glutDisplayFunc(Display);                                  //描画時に呼び出される関数を指定する（関数名：Display）
  Initialize();                                              //初期設定の関数を呼び出す
  glutMainLoop();
  return 0;
}
//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void Initialize(void){
  glClearColor(1.0, 1.0, 1.0, 0.0); //背景色
  glEnable(GL_DEPTH_TEST);//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する

  //透視変換行列の設定------------------------------
  glMatrixMode(GL_PROJECTION);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
  glLoadIdentity();//行列の初期化
  gluPerspective(30.0, (double)WindowWidth/(double)WindowHeight, 0.1, 1000.0); //透視投影法の視体積gluPerspactive(th, w/h, near, far);
  //------------------------------------------------
  gluLookAt(
       0.0, -200.0, 50.0, // 視点の位置x,y,z;
       0.0, 0.0, 20.0,   // 視界の中心位置の参照点座標x,y,z
       0.0, 0.0, 1.0);  //視界の上方向のベクトルx,y,z
}
//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void Idle(){
  glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}
//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去
  //モデルビュー変換行列の設定--------------------------
  glMatrixMode(GL_MODELVIEW);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
  glLoadIdentity();//行列の初期化
  glViewport(0, 0, WindowWidth, WindowHeight);
  //----------------------------------------------

  Ground();

  glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}
//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void Ground(void) {
    double ground_max_x = 300.0;
    double ground_max_y = 300.0;
    glColor3d(0.8, 0.8, 0.8);  // 大地の色
    // glColor3d(0, 0, 0);  // 大地の色
    glBegin(GL_LINES);
    for(double ly = -ground_max_y ;ly <= ground_max_y; ly+=20.0){
      glVertex3d(-ground_max_x, ly,0);
      glVertex3d(ground_max_x, ly,0);
    }
    for(double lx = -ground_max_x ;lx <= ground_max_x; lx+=20.0){
      glVertex3d(lx, ground_max_y,0);
      glVertex3d(lx, -ground_max_y,0);
    }
    glEnd();
}