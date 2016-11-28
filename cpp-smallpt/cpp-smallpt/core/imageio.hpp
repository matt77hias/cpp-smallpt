#pragma once

inline void WritePPM(int w, int h, const Vector3 *Ls, const string &fname = "cpp-image.ppm") {
	FILE *fp;
	fopen_s(&fp, fname.c_str(), "w");
	fprintf(fp, "P3\n%d %d\n%d\n", w, h, 255);
	for (size_t i = 0; i < w * h; ++i)
		fprintf(fp, "%d %d %d ", ToByte(Ls[i].x), ToByte(Ls[i].y), ToByte(Ls[i].z));
	fclose(fp);
}
