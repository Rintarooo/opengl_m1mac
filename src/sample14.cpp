#include <GL/glut.h>
#include <cmath>

#define X 0
#define Y 1
#define Z 2

float angle = 0.0;
unsigned int num_points = 8;
double point[][3] = {{1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}};
// unsigned int num_quads = 6;
// unsigned int quad[][4] = {{3, 2, 1, 0}, {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}, {4, 5, 6, 7}};
unsigned int num_triangles = 12;
unsigned int triangle[][3] = {{3, 2, 1}, {3, 1, 0}, {0, 1, 5}, {0, 5, 4}, {1, 2, 6}, {1, 6, 5}, {2, 3, 7}, {2, 7, 6}, {3, 0, 4}, {3, 4, 7}, {4, 5, 6}, {4, 6, 7}};

// 2本のベクトルvec0とvec1の内積．
double dot(double vec0[], double vec1[])
// double vec0[];
// double vec1[];
{
	return(vec0[X] * vec1[X] + vec0[Y] * vec1[Y] + vec0[Z] * vec1[Z]);
}

// 2本のベクトルvec0とvec1の外積．
void cross(double vec0[], double vec1[], double vec2[])
// double vec0[];
// double vec1[];
// double vec2[]; vec0×vec1.
{
	vec2[X] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	vec2[Y] = vec0[Z] * vec1[X] - vec0[X] * vec1[Z];
	vec2[Z] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
}

// ベクトルの正規化．
void normVec(double vec[])
// double vec[]; 注意！このベクトルは破壊的に変更される．
{
	double norm;
	norm = sqrt(vec[X] * vec[X] + vec[Y] * vec[Y] + vec[Z] * vec[Z]);
	vec[X] /= norm;
	vec[Y] /= norm;
	vec[Z] /= norm;
}

// 3頂点を含む平面の単位法線ベクトルの計算．
void normal(double p0[], double p1[], double p2[], double normal[])
// double p0[], p1[], p2[]; 凸ポリゴンの周囲に反時計周りに並ぶ3頂点の座標．
// double normal[]; 計算された法線ベクトル．
{
	unsigned int i;
	double v0[3], v1[3];

	// 基本となる２つのベクトルを生成．
	for (i = 0; i < 3; i++) {
		v0[i] = p2[i] - p1[i];
		v1[i] = p0[i] - p1[i];
	}

	// 生成したベクトルの外積を計算する．
	cross(v0, v1, normal);

	// 外積によって得られた法線ベクトルを正規化．
	normVec(normal);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//背景色で塗る

    // 光源の設定．
    // 同次座標系
    // float light_pos[4]
    // light_pos[3]=0だと、平行光線。light_pos[3]=1だと、放射状の光線。
	float light_pos[4] ={5.0f, 5.0f, 5.0f, 1.0f};
    // 光源の位置の定義
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);// GL_LIGHT0が光源の名前
	// 照明の点灯．
	glEnable(GL_LIGHTING);

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
    float camera_position[] = {5, 5, 5};
    float look_at[] = {-1.0f, -1.0f, -1};
    float yUp[]={0.0, 1.0, 0.0};

    // gluLookAt(5.0 * cos(angle), 3.0, 5.0 * sin(angle), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(camera_position[0], camera_position[1], camera_position[2],
              look_at[0], look_at[1], look_at[2],
              yUp[0], yUp[1], yUp[2]);

    // glColor3f(0.0, 0.0, 0.0);
    // // glutSolidCube(1.0);
    // glutWireCube(1.0);
    // // glutSolidSphere( GLdouble radius, GLdouble slices, GLdouble stacks ); 
    // glColor3f(0.0, 0.0, 0.0);
    // glutWireSphere(0.5, 15, 15); 
	glBegin(GL_TRIANGLES);
    
    double nrml_vec[3];
	unsigned int i;
	for (int i = 0; i < num_triangles; i++) {
		normal(point[triangle[i][0]], point[triangle[i][1]], point[triangle[i][2]], nrml_vec);
		glNormal3dv(nrml_vec);
        // glColor3f(0.0, 0.0, 255* i / num_triangles);
		glVertex3dv(point[triangle[i][0]]);
		glVertex3dv(point[triangle[i][1]]);
		glVertex3dv(point[triangle[i][2]]);
	}
	glEnd();

    glutSwapBuffers();
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


void initGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);// 背景色指定(白)
    glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);

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

    initGL();

    // glutDisplayFunc は、ウィンドウの描画に使用されるコールバック関数を登録する関数です。この関数に指定されたコールバック関数（この場合は display）は、ウィンドウが再描画される必要があるときに、GLUT によって自動的に呼び出されます。
    glutDisplayFunc(display);

    glutReshapeFunc(resize);
    // 無限ループで, プログラムはイベントの待ち受け状態になる
    glutMainLoop();
    return 0;
}