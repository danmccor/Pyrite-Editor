#include "WorldGrid.h"

GeomNode* WorldGrid::Create()
{
	lines.set_thickness(1);
	lines.set_color(LColor(0.6f, 0.6f, 0.6f, 1));

	for (int i = 0; i < 1000; i++) {
		if (i == 0) {
			//Draw Along Y
			lines.move_to(-1000, i, 0);
			lines.draw_to(1000, i, 0);

			//Draw Along X
			lines.move_to(i, -1000, 0);
			lines.draw_to(i, 1000, 0);

		}
		else {
			lines.move_to(-1000, i, 0);
			lines.draw_to(1000, i, 0);
			lines.move_to(-1000, -i, 0);
			lines.draw_to(1000, -i, 0);


			lines.move_to(i, -1000, 0);
			lines.draw_to(i, 1000, 0);
			lines.move_to(-i, -1000, 0);
			lines.draw_to(-i, 1000, 0);
		}
		
	}


	return lines.create();
}
