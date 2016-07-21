#include "header.h"
#include <time.h>
#include <Ws2tcpip.h>

void save(IplImage *image, int index){
	char *ch = new char;
	itoa(index, ch, 10);
	std::string filename = "c:/Users/Anna/Photos/" + (std::string)ch + (std::string)".jpg";
	cvSaveImage(&filename[0], image);
}

int* getAvailablePlaces(IplImage *image){
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	cvFindContours(image, storage, &contours);
	CvRect rect;
	double s = 0;
	double p = 0;
	int x;
	int y;
	int k;
	int l;
	int *arr = new int[16];//������ ��� ����������� ��������� ����: 1 -- ��������, 0 -- ������
	for(int i = 0; i<16; i++){
		arr[i] = 0;
	}
	for(CvSeq *seq = contours; seq != 0; seq = seq->h_next){
		s = cvContourArea(seq);
		p = cvArcLength(seq);
		if(abs(p*p/s) < 4*3.14 + 2 && s > 90){//��� ������� ������� ����������, �������� �� �� ����������� ������� �������
			rect = cvBoundingRect(seq);//������� ���������� ������ ����������� �������
			x = rect.x + rect.width/2;
			y = rect.y + rect.height/2;
			k = (x<130)?0:((x<170)?1:((x<345)?2:3));
			l = (y>160)?0:((y>120)?1:((y>80)?2:3));
			arr[k*4 + l] = 1;//�������� ����� � ������ ������������ ��� ���������
		}
	}
	return arr;
}

int getBestPlace(int *arr){
	int brr[] = {7, 5, 4, 3, 15, 13, 11, 9, 16, 14, 12, 10, 8, 6, 2, 1};//������ �������������� ����
	int max = 0;
	int index = -1;
	for(int i = 0; i<16; i++){//������� ����� �������� ������������� �� ��������� ����
		if(arr[i]*brr[i] > max){
			max = arr[i]*brr[i];
			index = i;
		}
	}
	return index + 1;//��������� ���� �� �������� ���������� � �������
}

void sendAnswer(SOCKET sct, int place){
	char *c = new char;
	itoa(place, c, 10);
	std::string s = "pad 0 " + (std::string)c + " 0\n";//������� ������ ����� � ������, ��������� ��� �������� ������
    send(sct, &s[0], s.size(), 0);
}

int main(){
	//������������ ����������
	int num = 2;
	char* ip[] = {"192.168.77.202", "192.168.77.204"};

	//��������� �������
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	int port_in = 8888;
	int port_out = 4444;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	SOCKET *sct_in = new SOCKET[num];//������ ��� ��������� �������� � ������ ������
	SOCKET *sct_out = new SOCKET[num];//������ ��� �������� ��������� ������ �� �����
	for(int i = 0; i<num; i++){
		addr.sin_addr.s_addr = inet_addr(ip[i]);
		addr.sin_port = htons(port_in);
		sct_in[i] = socket(AF_INET, SOCK_STREAM, 0);
		connect(sct_in[i],(SOCKADDR *) &addr, sizeof(addr));
		addr.sin_port = htons(port_out);
		sct_out[i] = socket(AF_INET, SOCK_STREAM, 0);
		connect(sct_out[i], (SOCKADDR *) &addr, sizeof(addr));
	}
	SOCKET sct_gate = socket(AF_INET, SOCK_STREAM, 0);//����� ��� ����������� � ���������
	addr.sin_addr.s_addr = inet_addr("192.168.77.1");
	addr.sin_port = htons(port_out);
	connect(sct_gate, (SOCKADDR *) &addr, sizeof(addr));
	
	char *buf = new char[11];
	char *s = "9:keepalive";//���������, ���������� �� ����� � ������� ����� ������� ����������
	int n = 0;

	//��������� �����������
	CvCapture *capture = cvCreateFileCapture("http://192.168.77.1:8080/?action=stream.mjpg");
	assert(capture);
	cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);
	IplImage *frame = 0;
	IplImage *image = 0;
	int fps = 30;
	long t;
	while(true){
		t = clock();
		while(clock() - t < 1000.0/fps){//�������� ��������� �� ��������� �����������
			t = clock();
			frame = cvQueryFrame(capture);
		}
		image = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);//�������������
		cvConvertImage(frame, image, CV_YUV2BGR);
		cvConvertImage(image, image, CV_BGR2GRAY);
		save(image, clock());
		cvSmooth(image, image, CV_GAUSSIAN, 3);
		cvThreshold(image, image, 120, 255, CV_THRESH_BINARY);
		cvErode(image, image);
		cvShowImage("capture", image);
		/* �������� �� ����� ��������� */
		for(int i = 0; i<num; i++){
			n = recv(sct_in[i], buf, 11, 0);
			if(n == -1){
				std::cout << "error: " << WSAGetLastError() << std::endl;
				break;
			}
			for(int j = 0; j<n; j++){
				if(buf[j] != s[j]){//���� ��������� ��������� ������� �� ����������� �� ���������
					for(int k = 0; k<n; k++){
						std::cout << buf[k];
					}
					std::cout << std::endl;
					n = getBestPlace(getAvailablePlaces(image));
					std::cout << n << std::endl;
					sendAnswer(sct_out[i], n);
					sendAnswer(sct_gate, n);
					break;
				}
			}
		}
		/* �������� �� ����� ��������� */
 		if(cvWaitKey(50) == 32){
			break;
		}
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("capture");

	//�������� �������
	for(int i = 0; i<num; i++){
		closesocket(sct_in[i]);
		closesocket(sct_out[i]);
	}
	closesocket(sct_gate);
	delete sct_in;
	delete sct_out;
	WSACleanup();
	return 0;
}