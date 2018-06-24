#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
typedef struct {
	float x;
	float y;

} Vector2f;

void print_bitmap_string(void* font, char* s)    //func. for displaying text
		{
	if (s && strlen(s)) {
		while (*s) {
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}

void MakeText(float x, float y, char * ch) {
	glRasterPos2f(x, y);
	print_bitmap_string(GLUT_BITMAP_HELVETICA_18, ch);
}

void RoundRect(float x, float y, float width, float height, float radius,
		int resolution) {
	float step = (2.0f * acos(-1)) / resolution, angle = 0.0f, x_offset,
			y_offset;

	int i = 0;

	unsigned int index = 0, segment_count = (int) (resolution / 4);

	Vector2f *top_left = (Vector2f *) malloc(segment_count * sizeof(Vector2f)),
			*bottom_left = (Vector2f *) malloc(
					segment_count * sizeof(Vector2f)), *top_right =
					(Vector2f *) malloc(segment_count * sizeof(Vector2f)),
			*bottom_right = (Vector2f *) malloc(
					segment_count * sizeof(Vector2f)), bottom_left_corner = { x
					+ radius, y - height + radius };

	while ((unsigned int) i != segment_count) {
		x_offset = cosf(angle);
		y_offset = sinf(angle);

		top_left[index].x = bottom_left_corner.x - (x_offset * radius);
		top_left[index].y = (height - (radius * 2.0f)) + bottom_left_corner.y
				- (y_offset * radius);

		top_right[index].x = (width - (radius * 2.0f)) + bottom_left_corner.x
				+ (x_offset * radius);
		top_right[index].y = (height - (radius * 2.0f)) + bottom_left_corner.y
				- (y_offset * radius);

		bottom_right[index].x = (width - (radius * 2.0f)) + bottom_left_corner.x
				+ (x_offset * radius);
		bottom_right[index].y = bottom_left_corner.y + (y_offset * radius);

		bottom_left[index].x = bottom_left_corner.x - (x_offset * radius);
		bottom_left[index].y = bottom_left_corner.y + (y_offset * radius);

		top_left[index].x = roundf(top_left[index].x);
		top_left[index].y = roundf(top_left[index].y);

		top_right[index].x = roundf(top_right[index].x);
		top_right[index].y = roundf(top_right[index].y);

		bottom_right[index].x = roundf(bottom_right[index].x);
		bottom_right[index].y = roundf(bottom_right[index].y);

		bottom_left[index].x = roundf(bottom_left[index].x);
		bottom_left[index].y = roundf(bottom_left[index].y);

		angle -= step;

		++index;

		++i;
	}

	glBegin(GL_TRIANGLE_STRIP);
	{
		// Top
		{
			i = 0;
			while ((unsigned int) i != segment_count) {
				//glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
				//glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
				glVertex2i(top_left[i].x, top_left[i].y);

				//glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
				//glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
				glVertex2i(top_right[i].x, top_right[i].y);

				++i;
			}
		}

		// In order to stop and restart the strip.
		//glColor4f( 0.0f, 1.0f, 0.0f,  1.5f );
		glVertex2i(top_right[0].x, top_right[0].y);

		//glColor4f( 0.0f, 1.0f, 0.0f,  1.5f );
		glVertex2i(top_right[0].x, top_right[0].y);

		// Center
		{
			//glColor4f( 0.0f, 1.0f, 0.0f,  1.0f );
			//glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
			glVertex2i(top_right[0].x, top_right[0].y);

			//glColor4f( 1.0f, 0.0f, 0.0f,  1.0f );
			//glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
			glVertex2i(top_left[0].x, top_left[0].y);

			//glColor4f( 0.0f, 0.0f, 1.0f,  1.0f );
			//glColor4f( 0.5f, 0.5f, 0.5f,  1.0f );
			glVertex2i(bottom_right[0].x, bottom_right[0].y);

			//glColor4f( 1.0f, 1.0f, 0.0f,  1.0f );
			//glColor4f( 0.5f, 0.5f, 0.5f,  1.0f );
			glVertex2i(bottom_left[0].x, bottom_left[0].y);
		}

		// Bottom
		i = 0;
		while ((unsigned int) i != segment_count) {
			//glColor4f( 0.0f, 0.0f, 1.0f,  1.0f );
			//glColor4f( 0.5f, 0.5f, 0.5f,  1.0f );
			glVertex2i(bottom_right[i].x, bottom_right[i].y);

			//glColor4f( 1.0f, 1.0f, 0.0f,  1.0f );
			//glColor4f( 0.5f, 0.5f, 0.5f,  1.0f );
			glVertex2i(bottom_left[i].x, bottom_left[i].y);

			++i;
		}
	}
	glEnd();

	glBegin(GL_LINE_STRIP);

//glColor4f( 0.0f, 1.0f, 1.0f, 1.0f );
//glColor4f( 1.0f, 0.5f, 0.0f, 1.0f );

// Border
	{
		i = (segment_count - 1);
		while (i > -1) {
			glVertex2i(top_left[i].x, top_left[i].y);

			--i;
		}

		i = 0;
		while ((unsigned int) i != segment_count) {
			glVertex2i(bottom_left[i].x, bottom_left[i].y);

			++i;
		}

		i = (segment_count - 1);
		while (i > -1) {
			glVertex2i(bottom_right[i].x, bottom_right[i].y);

			--i;
		}

		i = 0;
		while ((unsigned int) i != segment_count) {
			glVertex2i(top_right[i].x, top_right[i].y);

			++i;
		}

		// Close the border.
		glVertex2i(top_left[(segment_count - 1)].x,
				top_left[(segment_count - 1)].y);
	}
	glEnd();

	glBegin(GL_LINES);

//glColor4f( 0.0f, 1.0f, 1.0f, 1.0f );
//glColor4f( 0.0f, 0.5f, 1.0f, 1.0f );

// Separator
	{
		// Top bar
		glVertex2i(top_right[0].x, top_right[0].y);

		glVertex2i(top_left[0].x, top_left[0].y);

		// Bottom bar
		glVertex2i(bottom_left[0].x, bottom_left[0].y);

		glVertex2i(bottom_right[0].x, bottom_right[0].y);
	}
	glEnd();

	free(top_left);
	free(bottom_left);
	free(top_right);
	free(bottom_right);
}

void DrawCirle(float x, float y, float r) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i += 10)
		glVertex2f(x + r * cos(i * acos(-1) / 180),
				y + r * sin(i * acos(-1) / 180));
	glEnd();
}

class Player {
private:
	float posx, posy, wid, hei, stat;
public:
	enum State {
		Happy, Sad, Normal
	};

	Player(float posx, float posy, float height, float width) {
		Player::posx = posx;
		Player::posy = posy;
		Player::hei = height;
		Player::wid = width;
		stat = Normal;
	}

	~Player() {
	}

	void DrawFace() {
		glColor3f(255 / 255.0, 160 / 255.0, 122 / 255.0);
		RoundRect(posx - wid, posy + hei, wid * 2, hei * 2, wid/9, 64);
		glColor3f(255 / 255.0, 218 / 255.0, 185 / 255.0);
		DrawCirle(posx, posy, wid / 5);
		glColor3f(1, 1, 1);
		DrawCirle(posx - wid / 3, posy + hei / 3, wid / 3);
		DrawCirle(posx + wid / 3, posy + hei / 3, wid / 3);
		glColor3f(0, 0, 0);
		DrawCirle(posx - wid / 3, posy + hei / 5, wid / 5);
		DrawCirle(posx + wid / 3, posy + hei / 5, wid / 5);

		if (stat == Happy) {
			glColor3f(0, 0, 0);
			glBegin(GL_POLYGON);
			glVertex2f(posx - wid / 2.5f, posy - hei / 3);
			glVertex2f(posx, posy - hei / 1.5f);
			glVertex2f(posx + wid / 2.5f, posy - hei / 3);
			glEnd();
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glVertex2f(posx - wid / 2.8f, posy - hei / 3.3f);
			glVertex2f(posx, posy - hei / 1.8f);
			glVertex2f(posx + wid / 2.8f, posy - hei / 3.3f);
			glEnd();

		} else if (stat == Normal) {
			glColor3f(0, 0, 0);
			DrawCirle(posx, posy - hei / 2, wid / 5);
			glColor3f(1, 1, 1);
			DrawCirle(posx, posy - hei / 2, wid / 7);
		} else if (stat == Sad) {
			glColor3f(0, 0, 0);
			glBegin(GL_POLYGON);
			glVertex2f(posx - wid / 2.5f, posy - hei / 1.5f);
			glVertex2f(posx, posy - hei / 3);
			glVertex2f(posx + wid / 2.5f, posy - hei / 1.5f);
			glEnd();
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glVertex2f(posx - wid / 2.8f, posy - hei / 1.3f);
			glVertex2f(posx, posy - hei / 2.7f);
			glVertex2f(posx + wid / 2.8f, posy - hei / 1.3f);
			glEnd();
		}

	}

	void SetPos(float x, float y) {
		posx = x;
		posy = y;
	}

	void SetState(State S) {
		if (stat != Happy)
			stat = S;
	}

};
