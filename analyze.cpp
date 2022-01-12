	// Jacob Melone
#include "analyze.h"

typedef struct uuid{
	string hex_a = "", hex_b = "", hex_c = "", hex_d = "";
	void generate() {
		char alpha[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
		for (int i = 0; i < 16; i++) {
			srand(time(NULL));
			hex_a += alpha[rand() % 16];
		}
		for (int i = 0; i < 16; i++) {
			srand(time(NULL));
			hex_b += alpha[rand() % 16];
		}
		for (int i = 0; i < 16; i++) {
			srand(time(NULL));
			hex_c += alpha[rand() % 16];
		}
		for (int i = 0; i < 16; i++) {
			srand(time(NULL));
			hex_d += alpha[rand() % 16];
		}
	}
};

typedef struct pixel {
	uchar r, g, b;
	int size_x, size_y;
	int x, y;
};

/*typedef struct node{
	int x, y, z;
	uchar r, g, b;
	string type;
	vector<string> files;
	void debug_print();
	void read_from_file(string );
	void save_to_file(string);
};*/

/*void node::debug_print() {
	cout << setw(25) << "Node Type: " << node::type << endl
		<< setw(25) << "R,G,B: " << node::r << ", " << node::g << ", " << node::b << endl
		<< setw(25) << "Position: " << node::x << ", " << node::y << ", " << node::z << endl
		<< setw(25) << "Associated Files: ";
	for (int i = 0; i < node::files.size(); i++) {
		cout << setw(25) << node::files[i] << ",\n";
	}
}

void node::read_from_file(string file) {
	ifstream in;
	in.open(file, ios::in);
	uchar red, green, blue;
	int x_c, y_c, z_c;
	string filepath;
	while (in >> red >> green >> blue >> x_c >> y_c >> z_c >> filepath) {
		node::x = x_c;
		node::y = y_c;
		node::z = z_c;
		node::r = red;
		node::g = green;
		node::b = blue;
		ifstream inS;
		inS.open(filepath, ios::in);
		string path;
		while (inS >> path) {
			node::files.push_back(path);
		}
	}
	in.close();
}

void node::save_to_file(string file) {
	ofstream out;
	out.open(file, ios::out);
	uuid u;
	u.generate();
	out << setw(20) << node::r << setw(20) << node::g << setw(20) << node::b
		<< setw(20) << node::x << setw(20) << node::y << setw(20) << node::z
		<< setw(100) << "storedata/" << u.hex_a << "-" << u.hex_b << "-" << u.hex_c << "-" << u.hex_d << ".vibe" << endl;
	ofstream storedata;
	storedata.open("storedata/" + u.hex_a + "-" + u.hex_b + "-" + u.hex_c + "-" + u.hex_d + ".vibe", ios::out);
	for (int i = 0; i < node::files.size(); i++) {
		storedata << node::files[i] << endl;
	}
	out.close();
	storedata.close();
}

Vec3f div(Vec3f a, Vec3f b) {
	__m256 num = _mm256_set_ps(a[0], a[1], a[2], 1, 1, 1, 1, 1);
	__m256 den = _mm256_set_ps(b[0], b[1], b[2], 1, 1, 1, 1, 1);
	__m256 res = _mm256_div_ps(num, den);
	volatile float* r = (volatile float*)&res;
	Vec3f ret = (r[0], r[1], r[2]); 
	return ret;
}*/

void init() {
	string fp;
	cout << "Proceu Tech 2022 'Belgorod' Resolution Scale Detection\n\n";
	cout << setw(69) << "NOTE: Resolution is not exact, but gives rough estimate\n\n";
	cout << "Enter File Path: ";
	cin >> fp;
	cout << endl;
	VideoCapture cap(fp);
	if(!cap.isOpened()) {
		cout << "File not Found!\n";
	}
	bool end = false;
	ofstream csv;
	csv.open("ANALYSIS.csv");
	csv << "x_res,y_res\n";
	Mat frame;
	long count = 0;
	while (!end) {
		cap >> frame;
		/*vector<vector<bool>> MAP; MAP.resize(frame.rows);
		for (int y = 0; y < frame.rows; y++) {
			MAP[y].resize(frame.cols);
		}*/
		int x = 0, y = 0;
		if (frame.empty()) {
			end = true;
			break;
		}
		//MAP = contrast(frame);
		imshow("Video Input", frame);
		waitKey(1);
		pixel_count(frame, y, x);
		cout << setw(15) << "Frame " << count << ": " << setw(15) << x << "," << y << setw(15) << (float)x / frame.cols * 100 << "% X-Axis, " << setw(8) << (float)y / frame.rows * 100 << "% Y-Axis" << endl;
		csv << x << "," << y << "\n";
		count++;
	}
	csv.close();
}

/*vector<vector<bool>> contrast(Mat in) {
	Mat temp;
	in.convertTo(temp, CV_32FC3);
	float contrast = 1.0;
	int maxy = in.rows, maxx = in.cols;
	vector<vector<bool>> map;
	vector<vector<Vec3f>> div_res;
	div_res.resize(maxy);
	map.resize(maxy);
	Vec3f Tempa, Tempb, v;
	for (int y = 0; y < maxy; y++) {
		div_res[y].resize(maxx);
		for (int x = 0; x < maxx; x++) {
			Tempa = temp.at<Vec3f>(y,x);
			Tempb = temp.at<Vec3f>(y,abs(x - 1));
			div_res[y][x] = div(Tempa, Tempb);
		}
		map[y].resize(maxx);
	}
	for (int y = 0; y < maxy; y++) {
		for (int x = 0; x < maxx; x++) {
			v = temp.at<Vec3f>(y,x);
			if (v[0] != contrast && v[1] != contrast && v[2] != contrast) {
				map[y][x] = true;
				in.at<Vec3b>(y, x) = (255, 255, 255);
			}
			else {
				in.at<Vec3b>(y, x) = (0,0,0);
			}
		}
	}
	return map;
}
vector<vector<pixel>> break_down(Mat in) {
	vector<vector<pixel>> ret;
	for (int y = 0; y < in.size(); y++) {
		for (int x = 0; x < in[y].size(); x++) {
			if () {
				node n;
				n.x = x;
				n.y = y;
				n.z = 0;
				ret[y].push_back(n);
			}
			else if (in[y][abs(x - 1)]) {
				node n;
				n.x = x-1;
				n.y = y;
				n.z = 0;
				ret[y].push_back(n);
			}
			else if (in[y][x]) {
				node n;
				n.x = x;
				n.y = y;
				n.z = 0;
				ret[y].push_back(n);
			}
		}
	}
	return ret;
}*/

/*bool same(node a, node b) {
	if (a.x == b.x && a.y == b.y && a.z == b.z && a.r == b.r && a.g == b.g && a.b == b.b && a.type == b.type) {
		return true;
	}
	else {
		return false;
	}
}

node match(vector<node> in, node match) {
	for (int i = 0; i < in.size(); i++) {
		if (same(in[i], match)) {
			return in[i];
		}
	}
}*/

bool same_color(Vec3f a, Vec3f b, float percent) { 
	bool ret = false;
	if ((a[0] <= b[0] + (b[0] * percent) && b[0] <= a[0] + (a[0] * percent)) && (a[0] >= b[0] - (b[0] * percent) && b[0] >= a[0] - (a[0] * percent))) {
		ret = true;
	}
	else {
		return false;
	}
	if (a[1] <= b[1] + (b[1] * percent) && b[1] <= a[1] + (a[1] * percent) && (a[1] >= b[1] - (b[1] * percent) && b[1] >= a[1] - (a[1] * percent))) {
		ret = true;
	}
	else {
		return false;
	}
	if (a[2] <= b[2] + (b[2] * percent) && b[2] <= a[2] + (a[2] * percent) && (a[2] >= b[2] - (b[2] * percent) &&  b[2] >= a[2] - (a[2] * percent))) {
		ret = true;
	}
	else {
		return false;
	}
	return true;
}

bool interpolated(Vec3f a, Vec3f b, Vec3f c, float percent) {
	bool ret = false;
	__m128 a_m128 = _mm_set_ps((a[2] + ((c[2] - a[2])/ 2) - (a[2] * percent)), (a[1] + ((c[1] - a[2]) / 2) - (a[1] * percent)), (a[0] + ((c[0] - a[0]) / 2) - (a[0] * percent)), 1.0);
	__m128 b_m128 = _mm_set_ps(b[2], b[1], b[0], 1.0);
	__m128 c_m128 = _mm_set_ps((c[2]/2)+(c[2] * percent), (c[1] / 2) + (c[1] * percent), (c[0] / 2) + (c[0] * percent), 1.0);
	__m128 cmplt_b_c = _mm_cmplt_ps(b_m128, c_m128);		volatile bool* res_cmplt_b_c = (volatile bool*)&cmplt_b_c;
	__m128 cmpgt_a_b = _mm_cmpgt_ps(b_m128, a_m128);		volatile bool* res_cmpgt_a_b = (volatile bool*)&cmpgt_a_b;
	for (int i = 0; i < 3; i++) {
		if (res_cmplt_b_c[i] == 1 && res_cmpgt_a_b[i] == 1) {
			
		}
		else {
			return false;
		}
	}
	return true;
	/*if (b[0] / 2 <= ((c[0] - a[0]) / 2) + (((c[0] - a[0]) / 2) * percent) && b[0] >= ((c[0] - a[0]) / 2) - (((c[0] - a[0])) / 2 * percent)) {
		ret = true;
	}
	else {
		return false;
	}
	if (b[1] / 2 <= ((c[1] - a[1]) / 2) + (((c[1] - a[1]) / 2) * percent) && b[1] >= ((c[1] - a[1]) / 2) - (((c[1] - a[1]) / 2) * percent)) {
		ret = true;
	}
	else {
		return false;
	}
	if (b[2] / 2 <= ((c[2] - a[2]) / 2) + (((c[2] - a[2]) / 2) * percent) && b[2] >= ((c[2] - a[2]) / 2) - (((c[2] - a[2]) / 2) * percent)) {
		ret = true;
	}
	else {
		return false;
	}
	return true;*/
}

void pixel_count(Mat in /*vector<vector<bool>> in*/, int& y, int& x) {
	in.convertTo(in, CV_32FC3);
	int max_y = in.rows; int max_x = in.cols;
		// calculate pixel size
	//srand(time(NULL));
	//int r = rand() % 16 + 1;
	/*vector<vector<node>> ns = break_down(in);
	x = ns.size();
	y = ns[0].size();*/
	int pix_size = 0;
	int pix_size_p = 1024;
	int pix_size_max = 0;
	pixel current, prior;
	#pragma omp parallel for
	for (int xt = 0; xt < max_x; xt++) {
		//srand(time(NULL));
		//r = rand() % 16 + 1;
		if (interpolated(in.at<Vec3f>((max_y/2), xt), in.at<Vec3f>((max_y/2), abs(xt+1)%max_x), in.at<Vec3f>((max_y/2), abs(xt+2)%max_x), 0.000000001) || (same_color(in.at<Vec3f>((max_y/2), xt),in.at<Vec3f>((max_y/2),abs(xt+1)%max_x), 0.0001) && pix_size <= pix_size_max)) {
			pix_size++;
		}
		else {
			//cout << setw(20) << "X pixel size = " << pix_size_max << endl;
			if (pix_size > pix_size_max) {
				pix_size_max = pix_size;
			}

			pix_size_p = pix_size;
			pix_size = 0;
			x++;
		}
	}
	pix_size_max = 0;
	pix_size = 0;
	pix_size_p = 1024;
	#pragma omp parallel for
	for (int yt = 0; yt < max_y; yt++) {
		if (interpolated(in.at<Vec3f>(abs(yt-1), (max_x/2)), in.at<Vec3f>(yt, (max_x/2)), in.at<Vec3f>(abs(yt+1)%max_y, (max_x/2)), 0.000000001) || (same_color(in.at<Vec3f>(yt, (max_x / 2)), in.at<Vec3f>(abs(yt + 1) % max_y, (max_x / 2)), 0.0001) && pix_size <= pix_size_max)) {
			pix_size++;
		}
		else {
			//cout << setw(20) << "Y pixel size = " << pix_size_max << endl;
			if (pix_size > pix_size_max) {
				pix_size_max = pix_size;
			}
			pix_size_p = pix_size;
			pix_size = 0;
			y++;
		}
	}
	//y = y / pix_size;
	//x = x / pix_size;
}
