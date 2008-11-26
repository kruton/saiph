#include "Point.h"

/* constructors/destructor */
Point::Point(int row, int col) : row(row), col(col) {
}

/* operator overloading */
bool Point::operator<(const Point &point) const {
	return (row < point.row || (row == point.row && col < point.col));
}

bool Point::operator>(const Point &point) const {
	return (row > point.row || (row == point.row && col > point.col));
}

bool Point::operator<=(const Point &point) const {
	return (row < point.row || (row == point.row && col <= point.col));
}

bool Point::operator>=(const Point &point) const {
	return (row > point.row || (row == point.row && col >= point.col));
}

bool Point::operator==(const Point &point) const {
	return (row == point.row && col == point.col);
}

bool Point::operator!=(const Point &point) const {
	return (row != point.row || col != point.col);
}
