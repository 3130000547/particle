#include "Particle.h"
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//0��1���С��������
double Random()
{
    return (double)rand()/ (double)RAND_MAX;
}

//ȷ�����ӳ�ʼ�ٶȷ���ʹ��������ϵ��voxΪ�ٶ���xozƽ���ϵ�ͶӰ��x����ļнǣ�voyΪ�ٶ���y����ļн�
void RandomDirection(double vox, double voy, mVector3d *direction)
{
    direction->x = (double)(sin(voy) * sin(vox));
    direction->y = (double)cos(voy);
    direction->z = (double)(sin(voy) * cos(vox));
}

tEmitter::tEmitter()
{
}
tEmitter::~tEmitter()
{
    if(ptcPool)
        delete ptcPool;
}
//���������
void tEmitter::addParticle()
{
    tParticle *particle;
    mVector4d startColor, endColor;
    double voy, vox, speed;

    if (ptcPool != NULL && curPtc < totalPtc)
    {
        //�����ӿ���ȡ��һ���������뵱ǰ��Ծ����������
        particle = ptcPool;//���е�һ��Ԫ��
        ptcPool = ptcPool->next;

        if (ptc != NULL)
            ptc->prev = particle;
        particle->next = ptc;
        particle->prev = NULL;
        ptc = particle;

        //��һ���뾶Ϊ0.01�������ڵ�ĳƬ���������ȡһ��λ�ø���������ʼ��
        double degreeY = DEGTORAD(120.0 + 60.0 * Random());//begtorad
        double degreeXZ = DEGTORAD(360.0 * Random());
        particle->pos = mVector3d(0.001 * sin(degreeY) * cos(degreeXZ), 0.001 * (1 - cos(degreeY)), 0.001 * sin(degreeY) * sin(degreeXZ));
        particle->prevPos = particle->pos;	// �����߶�ģʽ

        // ���������ʼ�ٶ�����
        voy = (voy_max - voy_min) * Random();
        vox =  (vox_max - vox_min) * Random();;
        RandomDirection(vox,voy,&particle->direction);
        speed = speed_min + (speed_max  - speed_min) * Random();
        particle->direction = particle->direction * speed;

        // ����������ӳ�ʼ��ɫ����ֹ��ɫ
        startColor = startColor_min + (startColor_max - startColor_min) * Random();
        endColor =  endColor_min + (endColor_max - endColor_min) * Random();
        particle->currentColor = startColor;

        // ���������������ֵ����ֵ��ʾ���Ӵ��֡��
        particle->restLife = life_min + (life_max - life_min) * Random();

        // ����������ɫ�仯��
        particle->deltaColor= (endColor - startColor) *(1.0 / (double)particle->restLife);

        curPtc++;	//��������ǰ��������1
    }
}

//���Ӹ�����
void tEmitter:: updateParticle(tParticle *particle)
{
    if (particle != NULL && particle->restLife > 0)
    {
        particle->prevPos = particle->pos;
        particle->pos = particle->pos + particle->direction;
        particle->direction = particle->direction + force;

        particle->prevColor = particle->currentColor;
        //if(particle->restLife < 20)
            particle->currentColor = particle->currentColor + particle->deltaColor;

        particle->restLife--;
    }
    else if (particle != NULL && particle->restLife == 0)
    {
        //�޸ĵ�ǰ���ӵ�ǰ�����ӵ�ָ��
        if (particle->prev != NULL)
            particle->prev->next = particle->next;
        else
            ptc = particle->next;
        if (particle->next != NULL)
            particle->next->prev = particle->prev;
        //���������ӿ�
        particle->next = ptcPool;
        ptcPool = particle;
        curPtc--;
    }
}

//��������ʼ�����������ӿ��ʼ��
void ParticleSystem::initEmitter()
{
    srand((unsigned)time( NULL ));
    emitter = new tEmitter;
    emitter->totalPtc = 40000;
    emitter->ptcPool = (tParticle *)malloc(emitter->totalPtc * sizeof(tParticle));
    for (int i = 0; i < emitter->totalPtc - 1; i++)
    {
        emitter->ptcPool[i].next = &emitter->ptcPool[i + 1];
    }

    emitter->ptcPool[emitter->totalPtc - 1].next = NULL;

    //�ɸĳ��ⲿ����
    emitter->pos = mVector3d(0.0f, 0.0f, 0.0f);
    float m=Random();
    emitter->direction = mVector3d(m, m, m);

    emitter->voy_max =	DEGTORAD(360.0f);
    emitter->voy_min =	DEGTORAD(0.0f);
    emitter->vox_max =	DEGTORAD(360.0f);
    emitter->vox_min =  DEGTORAD(0.0f);
    emitter->speed_max =  0.006f;
    emitter->speed_min =  0.001f;

    emitter->curPtc	= 0;
    emitter->pfPtc_max	= 2000;
    emitter->pfPtc_min	= 1000;
    emitter->life_max = 45;
    emitter->life_min = 25;

    emitter->startColor_max = mVector4d(1.0f, 0.8f, 0.8f, 0.8f);
    emitter->startColor_min = mVector4d(0.9f, 0.72f, 0.8f, 0.8f);
    emitter->endColor_max = mVector4d(0.0f, 1.0f, 0.0f, 0.0f);
    emitter->endColor_min = mVector4d(0.0f, 0.8f, 0.0f, 0.0f);

    emitter->force = mVector3d(0.0f, 0.0006f, 0.0f);
    emitter->ptc	 = NULL;					// NULL TERMINATED LINKED LIST
}

//���·�����
void ParticleSystem::updateEmitter()
{
    tParticle *particle, *next;
    if (emitter != NULL)
    {
        if (emitter->ptc != NULL)
        {
            particle = emitter->ptc;
            while (particle)
            {
                next = particle->next;
                emitter->updateParticle(particle);
                particle = next;
            }
        }
        int emits = emitter->pfPtc_min + (emitter->pfPtc_max - emitter->pfPtc_min) * Random();
        for (int i = 0; i < emits; i++)
            emitter->addParticle();
    }
}

//��������ϵͳ������������ƻ���ģʽ����Ϊ1������ģʽ��Ϊ0���ǵ�ģʽ
void ParticleSystem::renderEmitter(bool antiAlias)
{
    updateEmitter();
    tParticle *particle = emitter->ptc;
    glPointSize(2.0);
    glLineWidth(2.0);
    if (antiAlias)
        glBegin(GL_LINES);
    else
        glBegin(GL_POINTS);
    while(particle)
    {
        if (antiAlias)
        {
            glColor4f(particle->prevColor.r, particle->prevColor.g, particle->prevColor.b,  particle->prevColor.a);
            glVertex3f(particle->prevPos.x,particle->prevPos.y,particle->prevPos.z);
        }
        glColor4f(particle->currentColor.r, particle->currentColor.g, particle->currentColor.b,  particle->currentColor.a);
        glVertex3f(particle->pos.x,particle->pos.y,particle->pos.z);
        particle = particle->next;
    }
    glEnd();
}

//ͨ�����������䷽��destinyȷ����ת���Լ���ת�Ƕ�degree������ת��Ĭ�����䷽���ǣ�0,1,0��
void TransAndRotate(mVector3d pos, mVector3d destiny)
{
    glTranslatef(pos.x, pos.y, pos.z);
    double axis[3];
    //axisΪ����destiny�루0,1,0���Ĳ��
    axis[0] = destiny.z;
    axis[1] = 0;
    axis[2] = -destiny.x;
    double degree = RADTODEG(acos(destiny.y/sqrt(destiny.x*destiny.x + destiny.y*destiny.y + destiny.z*destiny.z)));
    glRotated(degree, axis[0], axis[1], axis[2]);
}
