#pragma once

#include <stdio.h>

#include "vector.hpp"

namespace smallpt {

	inline void WritePPM(uint32_t w, uint32_t h, const Vector3 *Ls, const char *fname = "cpp-image.ppm") noexcept {
		FILE *fp;
		
		fopen_s(&fp, fname, "w");
		
		fprintf(fp, "P3\n%u %u\n%u\n", w, h, 255u);
		for (size_t i = 0; i < w * h; ++i) {
			fprintf(fp, "%u %u %u ", ToByte(Ls[i].m_x), ToByte(Ls[i].m_y), ToByte(Ls[i].m_z));
		}
		
		fclose(fp);
	}
}
