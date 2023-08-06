#include <glut.h>
#include <cmath>

float angle = 0.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//背景色で塗る
    glMatrixMode(GL_MODELVIEW);   /* マトリックスモードをモデルビューにする */ 
    glLoadIdentity();             /* 変換行列の初期化 */
    
    /*
    5.0 * cos(angle) および 5.0 * sin(angle) は、極座標系（距離と角度で位置を表す）から直交座標系（x, y, z座標で位置を表す）への変換
    極座標系では、点の位置は原点からの距離（半径）と角度で表されます。この例では、カメラの位置は原点から距離5.0の円周上にあります。`angle` は、カメラが円周上を移動する際の角度を表します。
    直交座標系への変換は、以下のように行われます：
    - x座標: x = r * cos(angle)
    - z座標: z = r * sin(angle)
    ここで、`r` は原点からの距離（この場合は5.0）、`angle` は極座標系での角度です。この変換により、カメラの位置が直交座標系で表現されます。
    この例では、y座標は3.0で一定です。これにより、カメラは立方体の上方から見下ろすような視点が維持されます。
    このように、極座標系から直交座標系への変換を行うことで、カメラが原点を中心に円軌道を描いて回転するような動きが実現されます
    */
    float camera_position[] = {5.0 * cos(angle), 4.0, 5.0 * sin(angle)};
    float look_at[] = {0.0f, 0.0f, 0.0f};
    float yUp[]={0.0, 1.0, 0.0};

    // gluLookAt(5.0 * cos(angle), 3.0, 5.0 * sin(angle), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(camera_position[0], camera_position[1], camera_position[2],
              look_at[0], look_at[1], look_at[2],
              yUp[0], yUp[1], yUp[2]);

    glColor3f(0.0, 0.0, 0.0);
    // glutSolidCube(1.0);
    glutWireCube(1.0);
    // glutSolidSphere( GLdouble radius, GLdouble slices, GLdouble stacks ); 
    glColor3f(0.0, 0.0, 0.0);
    glutWireSphere(0.5, 15, 15); 

    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity(); // 変換行列の初期化
    // gluPerspective(45.0, 1.0, 1.0, 100.0);


    glutSwapBuffers();
}

void timer(int value) {
    // angle += 0.02f; // カメラの回転速度を2倍にする
    angle += 0.10;

    // 角度 angle から 2π（360度）を減算して、角度を 0 から 2π の範囲に戻します。
    // これにより、角度が無限に大きくならず、常に一周の範囲内で回転することが保証されます。
    if (angle > 2 * M_PI) {
        angle -= 2 * M_PI;
    }

    // 要するに、`glutDisplayFunc(display); は display` 関数を描画のコールバック関数として登録する役割があり、
    // `glutPostRedisplay(); は登録された display` コールバック関数を呼び出してウィンドウを再描画する役割があります。
    // 両者は連携して、ウィンドウの描画と更新を適切に行うことができます。
    glutPostRedisplay();
    // この例では、`glutTimerFunc(16, timer, 0); としています。つまり、16ミリ秒後に timer` 関数が再び実行され、
    // この例では、16ミリ秒ごとに timer 関数が実行されるため、約60FPS（1秒間に約60回の更新）のアニメーションが実現されます。
    glutTimerFunc(16, timer, 0);
}

void resize(int w, int h){
    // ウィンドウが非常に狭くなるのを防ぐ
  if (h == 0) {
      h = 1;
  }

  const float aspect = (float)w / (float)h;
  
  glViewport(0, 0, w, h); // ウィンドウ全体をビューポートにする

//   glMatrixMode(GL_MODELVIEW);   /* マトリックスモードをモデルビューにする */ 
//   glLoadIdentity();             /* 変換行列の初期化 */
//   gluLookAt(0,0,0,
//             0,0,-1,
//             0,1,0);

  
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // 変換行列の初期化
    gluPerspective(45.0, aspect, 1.0, 10.0);
    // モデルビュー行列に戻す
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);// 背景色指定(白)
    glEnable(GL_DEPTH_TEST);
    // glMatrixMode(GL_PROJECTION);
    // gluPerspective(45.0, 1.0, 1.0, 10.0);
    // glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // - フロントバッファ: 現在画面に表示されている画像を保持しているバッファ
    // - バックバッファ: 次に表示する画像を描画しているバッファ
    // 描画プロセスでは、バックバッファに次のフレームの画像が描画され、描画が完了したら、`glutSwapBuffers();` が呼び出されてフロントバッファとバックバッファが入れ替わります。これにより、画面に新しいフレームが表示されます。入れ替えが行われると、新しいバックバッファで次のフレームの描画が始まります。
    // ダブルバッファリングを有効にするには、`glutInitDisplayMode()` 関数で GLUT_DOUBLE を指定します。
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("title");

    init();

    // glutDisplayFunc は、ウィンドウの描画に使用されるコールバック関数を登録する関数です。この関数に指定されたコールバック関数（この場合は display）は、ウィンドウが再描画される必要があるときに、GLUT によって自動的に呼び出されます。
    glutDisplayFunc(display);

    glutReshapeFunc(resize);


    // 1. unsigned int millis: 最初の引数は、コールバック関数が実行されるまでの待機時間（ミリ秒単位）を指定します。この例では、`0` が指定されているため、コールバック関数は即座に実行されます。
    // 2. void (*func)(int value): 2番目の引数は、タイマーが満了したときに実行されるコールバック関数へのポインタです。この例では、`timer` という関数が指定されています。
    // 3. int value: 3番目の引数は、コールバック関数に渡される整数値です。この例では、`0` が指定されています。
    glutTimerFunc(0, timer, 0);
    // 無限ループで, プログラムはイベントの待ち受け状態になる
    glutMainLoop();
    return 0;
}