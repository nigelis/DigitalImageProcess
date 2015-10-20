#include "ColorName.h"
#include <fstream>

/*!
Convert image to color names.

@param colorNames [in] vector<int> color names.
@param src [in] Mat, source image, CV_8UC3.
@param dst [out] Mat, result image, CV_8UC1, value 1~11.
@return int, 0x0000 = Successfully.
*/
int ConvertImage2ColorNames(vector<int>& colorNames, InputArray src, OutputArray dst)
{
	Mat source = src.getMat();
	if (source.empty() || source.type() != CV_8UC3)	return 0x0001;	/*!< Source image, not supported type. */
	
	dst.create(source.size(), CV_8UC1);
	Mat destination = dst.getMat();

	auto srcStart = source.begin<Vec3b>();
	auto srcEnd = source.end<Vec3b>();
	auto dstStart = destination.begin<uchar>();

	while (srcStart != srcEnd)
	{
		Vec3b bgr = *srcStart;
		int index = bgr[2] / 8 + bgr[1] / 8 * 32 + bgr[0] / 8 * 1024;
		*dstStart = colorNames[index];

		++srcStart;
		++dstStart;
	}

	return 0x0000;
}

/*!
Convert image to color.

@param colorNames [in] vector<int> color names.
@param src [in] Mat, source image, CV_8UC3.
@param dst [out] Mat, result image, CV_8UC3.
@return int, 0x0000 = Successfully.
*/
int ConvertImage2Color(vector<int>& colorNames, InputArray src, OutputArray dst)
{
	Mat source = src.getMat();
	if (source.empty() || source.type() != CV_8UC3)	return 0x0001;	/*!< Source image, not supported type. */

	dst.create(source.size(), CV_8UC3);
	Mat destination = dst.getMat();

	const uchar COLOR_VALUES[11][3] = {{0, 0, 0},{255, 0, 0},{64, 102, 128},{128, 128, 128},{0, 255, 0},{0, 204, 255},{255, 128, 255},{255, 0, 255},{0, 0, 255},{255, 255, 255},{0, 255, 255}};

	auto srcStart = source.begin<Vec3b>();
	auto srcEnd = source.end<Vec3b>();
	auto dstStart = destination.begin<Vec3b>();

	while (srcStart != srcEnd)
	{
		Vec3b bgr = *srcStart;
		int index = bgr[2] / 8 + bgr[1] / 8 * 32 + bgr[0] / 8 * 1024;
		int color = colorNames[index];
		*dstStart = Vec3b(COLOR_VALUES[color - 1]);

		++srcStart;
		++dstStart;
	}

	return 0x0000;
}

/*!
Read color indexs from specified file.

@param filePath [in] string, file path.
@param colorNames [out] vector<int>, color indexs.
@return int, 0x0000 = read successfully.
*/
int ReadConfigs(string filePath, vector<int>& colorNames)
{
	fstream f = fstream(filePath, ios::in);
	if (!f.is_open()) return 0x0001;	/*!< File is not opened. */

	const int COLOR_ROW_COUNTS = 32768;
	const int COLOR_COL_COUNTS = 11;

	colorNames.resize(COLOR_ROW_COUNTS);
	int readCounts = 0;

	while (!f.eof())
	{
		int color = 0;
		double value, maxValue = 0;

		// abandon rgb color values.
		f >> value;
		f >> value;
		f >> value;
		if (f.eof()) break;

		for (int i = 0; i < COLOR_COL_COUNTS; i++)
		{
			f >> value;

			if (value > maxValue)
			{
				maxValue = value;
				color = i + 1;
			}
		}

		colorNames[readCounts] = color;
		readCounts++;
	}

	if (readCounts != COLOR_ROW_COUNTS)	return 0x0002;	/*!< Not enough configs. */
	return 0x0000;	/*!< Successfully. */
}

/*!
Read color probability indexs from specified file.

@param filePath [in] string, file path.
@param colorNames [out] vector<int>, color probability indexs.
@return [int] 0x0000 = read successfully.
*/
int ReadConfigs(string filePath, vector<vector<double>>& colorNames)
{

	fstream f = fstream(filePath, ios::in);
	if (!f.is_open()) return 0x0001;	/*!< File is not opened. */

	const int COLOR_ROW_COUNTS = 32768;
	const int COLOR_COL_COUNTS = 11;

	colorNames.resize(COLOR_ROW_COUNTS);
	int readCounts = 0;

	while (!f.eof())
	{
		vector<double> color;
		double value;

		// abandon rgb color values.
		f >> value;
		f >> value;
		f >> value;
		if (f.eof()) break;

		for (int i = 0; i < COLOR_COL_COUNTS; i++)
		{
			f >> value;
			color.push_back(value);
		}

		colorNames[readCounts] = color;
		readCounts++;
	}

	if (readCounts != COLOR_ROW_COUNTS)	return 0x0002;	/*!< Not enough configs. */
	return 0x0000;	/*!< Successfully. */
}