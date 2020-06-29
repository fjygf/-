#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>//��������Ҫ
using namespace std;

/*��������֮��ľ�����xzƽ���ϵ�ͶӰ*/
double xz_distance(double a[], double b[]) {
	return  sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
}
/*������*/
class Tooth//���������汣���������Ƭ��m_FacetList���������������ɽ�ʡ�ڴ�
{
public:
	class AABB //Բ����AABB��Χ��
	{
	public:
		double R, y_up, y_down;//����뾶��Բ���嶥��y���ꡢԲ����ײ�y����
		double center[3];//���ĵ�
		AABB() {}
	};
	int original_order;
	AABB aabb;
	int name;//���ݱ��
	bool isNull;//�Ƿ�Ϊ������
	Tooth() {}

	//���ĵ��xֵԽС���ͱ�����Ϊ����Խ��<��
	bool operator<(Tooth t) {
		if (this->aabb.center[0] < t.aabb.center[0]) return true;
		else return false;
	}
};

/*����һϵ��bounds������boundsԭ�ȵ�˳�򣬷��ض�Ӧ�����ݱ��*/
	//up_or_low��ʾ�����ģ�������(����0)�������(����1); num_of_realTooth����ͨ�����ĸ�����
int* identify(int num_of_realTooth, double** bound, int up_or_low) {
	/*�������ݶ���*/
	vector<Tooth>TeethList;//���ݶ���
	int right1;//��һ��������������
	for (int i = 0; i < num_of_realTooth; i++) {
		Tooth tmp;
		tmp.aabb.center[0] = (bound[i][0] + bound[i][1]) * 0.5;//x
		tmp.aabb.center[1] = (bound[i][2] + bound[i][3]) * 0.5;//y
		tmp.aabb.center[2] = (bound[i][4] + bound[i][5]) * 0.5;//z
		tmp.aabb.R = 0.5 * sqrt((bound[i][1] - bound[i][0]) * (bound[i][1] - bound[i][0]) + (bound[i][5] - bound[i][4]) * (bound[i][5] - bound[i][4]));
		tmp.isNull = false;
		tmp.aabb.y_up = bound[i][3];
		tmp.aabb.y_down = bound[i][2];
		tmp.original_order = i;//��ʼ˳��
		TeethList.push_back(tmp);
	}
	sort(TeethList.begin(), TeethList.end());//�����������ĵ��x��ֵ�����ݽ�������
/*��������ɨ��������ݲ���*/
	//��ƿ�������Ϊ��Ӧ��������Ϊ�յ����
	//���������̲�������ݽ������ڲ�ȱ��ʱ�����������߻��������ұߵ�ĥ��ȱʧ���򲹲���
	for (int i = 0; i < num_of_realTooth - 1; i++) {
		double sum_of_R = (TeethList[i].aabb.R + TeethList[i + 1].aabb.R);
		double xz_d = xz_distance(TeethList[i].aabb.center, TeethList[i + 1].aabb.center);
		if (xz_d > sum_of_R) {  //����ȱ��
			Tooth empty;		//����һ��������

					//�������ڲ�����ֵ�ĸ�ֵ
			empty.aabb.center[0] = (TeethList[i].aabb.center[0] + TeethList[i + 1].aabb.center[0]) * 0.5;
			empty.aabb.center[1] = (TeethList[i].aabb.center[1] + TeethList[i + 1].aabb.center[1]) * 0.5;
			empty.aabb.center[2] = (TeethList[i].aabb.center[2] + TeethList[i + 1].aabb.center[2]) * 0.5;
			empty.aabb.R = (xz_d - sum_of_R) * 0.5;
			empty.aabb.y_down = (TeethList[i].aabb.y_down + TeethList[i + 1].aabb.y_down) * 0.5;
			empty.aabb.y_up = (TeethList[i].aabb.y_up + TeethList[i + 1].aabb.y_up) * 0.5;
			empty.isNull = true;
			//�����ݲ�����original_order

			TeethList.push_back(empty);//���������
		}
	}
	sort(TeethList.begin(), TeethList.end());//����˿����ݺ����¶����ݶ��а���center.x�Ĵ�С��������

/*�ҳ�center.x��������center.x��С���ǿ����ݵ��±꣬���Ұ�ߵĵ�һ������*/
	//����������е�ǰ���ǣ�����ģ�͵�z������ģ���м�
	right1 = TeethList.size() - 1;//��ʼ��right1Ϊ���ұߵ�����
	for (int i = 0; i < TeethList.size(); i++) {
		if (TeethList[i].aabb.center[0] > 0) {
			right1 = i;
			break;//��Ϊ�Ѿ��ź����ˣ���һ��������ļ�Ϊ����
		}
	}

	/*���ݵ�������ʹ��FDI��λ��ʾ��*/
		//(�Բ��˵ĽǶȶ�������)
	int quadrant;//��ʾ���ݴ����ĸ����ޣ�
					//���ϣ�quadrant = 10
					//���ϣ�quadrant = 20
					//���£�quadrant = 30
					//���£�quadrant = 40
	//�Ҳ�������
	if (up_or_low == 0) quadrant = 10;//����
	else quadrant = 40;//����
	for (int i = right1; i < TeethList.size(); i++) {
		TeethList[i].name = quadrant + i - right1 + 1;
		//cout << "�Ҳ���������" << model.TeethList[i].name << endl;
	}
	//���������
	if (up_or_low == 0) quadrant = 20;//����
	else quadrant = 30;//����
	for (int i = right1 - 1; i >= 0; i--) {
		TeethList[i].name = quadrant + right1 - i;
		//cout << "�����������" << model.TeethList[i].name << endl;
	}
	/*���ر��*/
	int *bianhao = new int[16];//�����Ƕ�̬����
	for (int i = 0; i < TeethList.size(); i++) {
		if (TeethList[i].isNull == true) continue;//����������
		bianhao[TeethList[i].original_order] = TeethList[i].name;
	}
	return bianhao;//���ر�������Ӧ��ָ��
}