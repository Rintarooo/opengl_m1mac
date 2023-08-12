// 3-8 shading


#include <GL/glut.h>
#include <math.h>

#define X 0
#define Y 1
#define Z 2

// �~�����D
#define PI 3.141592653589793

unsigned int num_points = 8;
double point[][3] = {{1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}};
unsigned int num_quads = 6;
unsigned int quad[][4] = {{3, 2, 1, 0}, {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}, {4, 5, 6, 7}};
unsigned int num_triangles = 12;
unsigned int triangle[][3] = {{3, 2, 1}, {3, 1, 0}, {0, 1, 5}, {0, 5, 4}, {1, 2, 6}, {1, 6, 5}, {2, 3, 7}, {2, 7, 6}, {3, 0, 4}, {3, 4, 7}, {4, 5, 6}, {4, 6, 7}};

unsigned int window_width;
unsigned int window_height;

double eye[3];
double center[3] = {0.0, 0.0, 0.0};
double up[3];

double phi = 30.0;
double theta = 30.0;

float light_pos[4];


// 2�{�̃x�N�g��vec0��vec1�̓��ρD
double dot(double vec0[], double vec1[])
// double vec0[];
// double vec1[];
{
	return(vec0[X] * vec1[X] + vec0[Y] * vec1[Y] + vec0[Z] * vec1[Z]);
}

// 2�{�̃x�N�g��vec0��vec1�̊O�ρD
void cross(double vec0[], double vec1[], double vec2[])
// double vec0[];
// double vec1[];
// double vec2[]; vec0�~vec1.
{
	vec2[X] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	vec2[Y] = vec0[Z] * vec1[X] - vec0[X] * vec1[Z];
	vec2[Z] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
}

// �x�N�g���̐��K���D
void normVec(double vec[])
// double vec[]; ���ӁI���̃x�N�g���͔j��I�ɕύX�����D
{
	double norm;
	norm = sqrt(vec[X] * vec[X] + vec[Y] * vec[Y] + vec[Z] * vec[Z]);
	vec[X] /= norm;
	vec[Y] /= norm;
	vec[Z] /= norm;
}

// 3���_���܂ޕ��ʂ̒P�ʖ@���x�N�g���̌v�Z�D
void normal(double p0[], double p1[], double p2[], double normal[])
// double p0[], p1[], p2[]; �ʃ|���S���̎��͂ɔ����v����ɕ���3���_�̍��W�D
// double normal[]; �v�Z���ꂽ�@���x�N�g���D
{
	unsigned int i;
	double v0[3], v1[3];

	// ��{�ƂȂ�Q�̃x�N�g���𐶐��D
	for (i = 0; i < 3; i++) {
		v0[i] = p2[i] - p1[i];
		v1[i] = p0[i] - p1[i];
	}

	// ���������x�N�g���̊O�ς��v�Z����D
	cross(v0, v1, normal);

	// �O�ςɂ���ē���ꂽ�@���x�N�g���𐳋K���D
	normVec(normal);
}

// ���e�����p�̍s���`�D���f�����E�B���h�E��t�ɕ\������D
void defineViewMatrix(double phi, double theta)
// double phi, theta; ���ʊp�Ƌp.
{
	unsigned int i, j;
	double c, s, xy_dist;
	double x_axis[3], y_axis[3], z_axis[3], vec[3];
	double left, right, bottom, top, farVal, nearVal, margin;
	double dx, dy, d_aspect, w_aspect, d;

	// ���_�̐ݒ�D
	eye[Z] = sin(theta * PI / 180.0);
	xy_dist = cos(theta * PI / 180.0);
	c = cos(phi * PI / 180.0);
	s = sin(phi * PI / 180.0);
	eye[X] = xy_dist * c;
	eye[Y] = xy_dist * s;
	up[X] = - c * eye[Z];
	up[Y] = - s * eye[Z];
	up[Z] = s * eye[Y] + c * eye[X];
	normVec(up);

	// ���_�����_�Ƃ�����W�n�̒�`�D
	for (i = 0; i < 3; i++)
		z_axis[i] = eye[i] - center[i];
	normVec(z_axis);
	cross(up, z_axis, x_axis);
	normVec(x_axis);
	cross(z_axis, x_axis, y_axis);

	// left, right, bottom, top, nearVal, farVal�̌���D
	left = bottom = farVal = 10000.0;
	right = top = nearVal = -10000.0;
	for (i = 0; i < num_points; i++) {
		for (j = 0; j < 3; j++)
			vec[j] = point[i][j] - eye[j];		
		if (dot(x_axis, vec) < left)
			left = dot(x_axis, vec);
		if (dot(x_axis, vec) > right)
			right = dot(x_axis, vec);
		if (dot(y_axis, vec) < bottom)
			bottom = dot(y_axis, vec);
		if (dot(y_axis, vec) > top)
			top = dot(y_axis, vec);
		if (dot(z_axis, vec) < farVal)
			farVal = dot(z_axis, vec);
		if (dot(z_axis, vec) > nearVal)
			nearVal = dot(z_axis, vec);
	}

	// �}�`�̎��͂�5%�قǗ]�T��^����D
	margin = (right - left) * 0.05;
	left -= margin;
	right += margin;
	margin = (top - bottom) * 0.05;
	bottom -= margin;
	top += margin;
	margin = (nearVal - farVal) * 0.05;
	farVal -= margin;
	nearVal += margin;

	// �\���͈͂̃A�X�y�N�g��ƃE�B���h�E�̃A�X�y�N�g��̔�r�D
	dx = right - left;
	dy = top - bottom;
	d_aspect = dy / dx;
	w_aspect = (double)window_height / (double)window_width;

	// �E�B���h�E���\���͈͂����c���D�\���͈͂��c�ɍL����D
	if (w_aspect > d_aspect) {
		d = (dy * (w_aspect / d_aspect - 1.0)) * 0.5;
		bottom -= d;
		top += d;

		// �E�B���h�E���\���͈͂��������D�\���͈͂����ɍL����D
	} else {
		d = (dx * (d_aspect / w_aspect - 1.0)) * 0.5;
		left -= d;
		right += d;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, - nearVal, - farVal);
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[X], eye[Y], eye[Z], center[X], center[Y], center[Z], up[X], up[Y], up[Z]); 
}

void display(void)
{
	unsigned int i;
	double nrml_vec[3];

	// �����̐ݒ�D
	light_pos[0] = (float)eye[X];
	light_pos[1] = (float)eye[Y];
	light_pos[2] = (float)eye[Z];
	light_pos[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);// GL_LIGHT0�������̖��O

	// �Ɩ��̓_���D
	glEnable(GL_LIGHTING);

	// �����e�̒�`�D
	defineViewMatrix(phi, theta);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < num_triangles; i++) {
		normal(point[triangle[i][0]], point[triangle[i][1]], point[triangle[i][2]], nrml_vec);
		glNormal3dv(nrml_vec);
		glVertex3dv(point[triangle[i][0]]);
		glVertex3dv(point[triangle[i][1]]);
		glVertex3dv(point[triangle[i][2]]);
	}
	glEnd();
	glFlush();
}

void resize(int width, int height)
{

	// �E�B���h�E�T�C�Y�̎擾�D
	window_width = width;
	window_height = height;
}

void initGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
}

int main(int argc, char *argv[])
{
	glutInitWindowPosition(128, 128); // �E�B���h�E�̏����ʒu�D
	glutInitWindowSize(768, 768); // �E�B���h�E�T�C�Y���`�D
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	initGL();
	glutMainLoop();
	return 0;
}
