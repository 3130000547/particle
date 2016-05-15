#ifndef PARTICLE_H
#define PARTICLE_H
#define M_PI        3.14159265358979323846
#define DEGTORAD(d)	((d * (double)M_PI) / 180.0f);
#define RADTODEG(r)	((r * 180.0f) /(double)M_PI);

class mVector3d
{
public:
    mVector3d():x(0), y(0), z(0){}  //���ֳ�ʼ��
    mVector3d(double x1, double y1, double z1):x(x1), y(y1), z(z1){}
    mVector3d(const mVector3d &v){ x = v.x;  y = v.y;  z = v.z; }//���ֳ�ʼ��������
    ~mVector3d(){}
    void operator=(const mVector3d &v){  x = v.x;  y = v.y;  z = v.z; }
    mVector3d operator+(const mVector3d &v){  return mVector3d(x+v.x, y+v.y, z+v.z);  }
    mVector3d operator-(const mVector3d &v){  return mVector3d(x-v.x, y-v.y, z-v.z);  }
    mVector3d operator+(double d){  return mVector3d(x+d, y+d, z+d);  }
    mVector3d operator-(double d){  return mVector3d(x-d, y-d, z-d);  }
    mVector3d operator*(double d){  return mVector3d(x*d, y*d, z*d);  }  //�����������ص�д��
public:
    double x, y, z;
};

class mVector4d
{
public:
    mVector4d():r(0), g(0), b(0), a(0){}
    mVector4d(double r, double g, double b, double a):r(r), g(g), b(b), a(a){}
    ~mVector4d(){}
    void operator=(const mVector4d &v){  r = v.r;  g = v.g;  b = v.b; a = v.a; }
    mVector4d operator+(const mVector4d &v){  return mVector4d(r+v.r, g+v.g, b+v.b, a+v.a);  }
    mVector4d operator-(const mVector4d &v){  return mVector4d(r-v.r, g-v.g, b-v.b, a-v.a);  }
    mVector4d operator+(double d){  return mVector4d(r+d, g+d, b+d, a+d);  }
    mVector4d operator-(double d){  return mVector4d(r-d, g-d, b-d, a-d);  }
    mVector4d operator*(double d){  return mVector4d(r*d, g*d, b*d, a*d);  }
public:
    double r, g, b, a;
};

struct tParticle
{
    tParticle   *prev,*next;			// ���Ӽ���������ʽ����
    mVector3d	pos;					// ��ǰλ��
    mVector3d	prevPos;				// ��ǰλ��
    mVector3d	direction;				// ��ǰ�ٶȷ���
    int		    restLife;				// ʣ����ʱ��
    mVector4d	currentColor;			// ��ǰ��ɫ
    mVector4d	prevColor;				// ��ǰ��ɫ
    mVector4d	deltaColor;				// ��ɫ�仯��
};

class tEmitter
{
public:
    tEmitter();
    ~tEmitter();
    void addParticle();
    void updateParticle(tParticle *particle);
public:
    mVector3d	pos;							// ������λ��
    mVector3d    direction;						// ����������������
    //ͨ��������ϵ�������ָ������Χ�ڵ��ٶ�����
    double		voy_max, voy_min;				// ���ӳ�ʼ�����ٶ�����y��ļнǷ�Χ
    double		vox_max, vox_min;				// ���ӳ�ʼ�����ٶ�����X��ļнǷ�Χ
    double		speed_max, speed_min;			// ���ӳ�ʼ�ٶȱ仯��Χ
    // ����������Ϣ
    tParticle   *ptcPool;
    tParticle	*ptc;							// ָ��ǰ��Ծ��������
    int			totalPtc;						// ���ӿ���������
    int			curPtc;							// ��ǰ������
    int			pfPtc_max, pfPtc_min;			// ÿһ֡����������仯��Χ
    int			life_max, life_min;				// ���ӳ�ʼ����ֵ�仯��Χ
    mVector4d	startColor_max, startColor_min;	// ��ʼ��ɫ�仯��Χ
    mVector4d	endColor_max, endColor_min;		// ��ʧ��ɫ�仯��Χ
    //����������Ϣ
    mVector3d	force;							// ���������ٶȱ仯��
};

class ParticleSystem
{
public:
    void initEmitter();
    void updateEmitter();
    void renderEmitter(bool antiAlias);

public:
    tEmitter *emitter;
};

void TransAndRotate(mVector3d pos, mVector3d destiny);



#endif // PARTICLE_H

