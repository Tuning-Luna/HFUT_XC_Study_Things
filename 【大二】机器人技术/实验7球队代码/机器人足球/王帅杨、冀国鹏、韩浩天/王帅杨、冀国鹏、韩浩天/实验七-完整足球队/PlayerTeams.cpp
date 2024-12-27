/*
Copyright (c) 2000-2003, Jelle Kok, University of Amsterdam
All rights reserved.
Modified by ming gao Hfut..for server V14
1). dash dirction for not only forward and back but also left right and other 45*(-3,-1,1,3) dirction,,
2). add Stamina.Capacity and the senceHandle analy systerm and worldmodel update
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
3. Neither the name of the University of Amsterdam nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! \file PlayerTeams.cpp
<pre>
<b>File:</b>          PlayerTest.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       10/12/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class definitions for the
                      Player that are used to test the teams' high level
                      strategy.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
10/12/2000        Jelle Kok       Initial version created
</pre>
*/

#include "Player.h"
#include "math.h"
using namespace std;

/*!This method is the first complete simple team and defines the actions taken
   by all the players on the field (excluding the goalie). It is based on the
   high-level actions taken by the simple team FC Portugal that it released in
   2000. The players do the following:
   - if ball is kickable
       kick ball to goal (random corner of goal)
   - else if i am fastest player to ball
       intercept the ball
   - else
       move to strategic position based on your home position and pos ball */
///misol.gao fixed--->
///��Һã��ܸ��������ҵ�������������������������������Ӿ��ߵ���㣬���еı������Զ����������������������ģ�
/// deMeer5 ����Ա�Ĳ��Ժ����� ���������ǻᷢ�� deMeer5_goalie �Ǹ�������Ա�ģ�Ŀǰ�������Ժ��ԡ�
SoccerCommand Player::deMeer5(  )
{

    SoccerCommand soc(CMD_ILLEGAL);// ����һ��������󣬺����Ƶ������������һ����Ա��������
    VecPosition   posAgent = WM->getAgentGlobalPosition();// һ������󣬱�ʾAgent(�������ɵ�ǰ��ִ������������Ա���Լ�)��λ��
    ///�������ǰѡ��ҡ������ɵ�ǰִ����������Agent����Ϊ��11����Ա��ִ��������룬���Զ���ÿ����Ա �϶���һ�����ҡ����ڡ�
    VecPosition   posBall  = WM->getBallPos();// ���λ��
    VecPosition   posGoal  = WM->getPosOpponentGoal();// ���λ��
    int           iTmp;


    if ( WM->isBeforeKickOff( ) ) /// �����û�п��� ע�⣡ ����ֻ�ǻ�û�п��������� Ҫ�������ľ��ߣ����������������һ�� WM->isBeforeKickOff( )
    {
        if ( WM->isKickOffUs( ) && WM->getPlayerNumber() == 9 ) // 9 takes kick //����9��ȥ�����ж����ǲ������ǿ��򣬺���"Agent"�ǲ���9�ţ�
        {
            if ( WM->isBallKickable() )// �����������˵�ǲ��ǿ��ߣ�
            {
		    VecPosition posGoal(PITCH_LENGTH/2.0, 
				       (-1 + 2*(WM->getCurrentCycle()%2)) * 
				       0.4 * SS->getGoalWidth());
		    soc=kickTo( posGoal,SS->getBallSpeedMax());// �����ŷ����������������
			Log.log(100, "take kick off");

            }
            else //���������Ҳ����ߣ����Ҿ�ȥ����
            {
                soc = intercept( false );
                Log.log( 100, "move to ball to take kick-off" );
            }
            ACT->putCommandInQueue( soc );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            return soc;
        }
        //���治�ùܣ�
        if ( formations->getFormation() != FT_INITIAL || // not in kickoff formation
                posAgent.getDistanceTo( WM->getStrategicPosition() ) > 2.0 )
        {
            formations->setFormation( FT_INITIAL );       // go to kick_off formation
            ACT->putCommandInQueue( soc=teleportToPos( WM->getStrategicPosition() ));
        }
        else                                            // else turn to center
        {
            ACT->putCommandInQueue( soc=turnBodyToPoint( VecPosition( 0, 0 ), 0 ) );
            ACT->putCommandInQueue( alignNeckWithBody( ) );
        }
    }
    
	//��������ģʽ����
    else if( WM->isCornerKickUs())//�ҷ�����
    {
	if(WM->isBallKickable() )
	{
	    soc = kickTo(pointKickToC(), 2.0);
	}
	else if(WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp)
	  == WM->getAgentObjectType())
	{
	    soc = moveToPos(posBall, PS->getPlayerWhenToTurnAngle());
	}
	else if(WM->getSecondClosestInSetTo(OBJECT_SET_TEAMMATES, OBJECT_BALL)
	  == WM->getAgentObjectType())
	{
	    soc = moveToPos(pointKickToC(), PS->getPlayerWhenToTurnAngle());
	}
	else
	{
	    soc = moveToPos(WM->getStrategicPosition(), PS->getPlayerWhenToTurnAngle());
	}
	
	ACT->putCommandInQueue( soc ); // �����������
        ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    
    else if( WM->isKickInUs() )//�ҷ�������
    {
	if( WM->isBallKickable() )
	{
	    soc = kickTo(pointKickToK(), 2.0);
	}
	else if(WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp)
	  == WM->getAgentObjectType())
	{
	    soc = moveToPos(posBall, PS->getPlayerWhenToTurnAngle());
	}
	else if(WM->getSecondClosestInSetTo(OBJECT_SET_TEAMMATES, OBJECT_BALL)
	  == WM->getAgentObjectType())
	{
	    soc = moveToPos(pointKickToK(), PS->getPlayerWhenToTurnAngle());
	}
	else
	{
	    soc = moveToPos(WM->getStrategicPosition(), PS->getPlayerWhenToTurnAngle());
	}
	
	ACT->putCommandInQueue( soc ); // �����������
        ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    
    else if( WM->isFreeKickUs() )//�ҷ�������
    {
	if( WM->isBallKickable() )
	{
	    if(WM->getBallPos().getX() > 35)//x����35ֱ������
	    {
		soc = maxAngShoot( posAgent );
	    }
	    else
	    {
		soc = kickTo(pointKickToK(), 2.0);		
	    }

	}
	else if(WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp)
	  == WM->getAgentObjectType())
	{
	    soc = moveToPos(posBall, PS->getPlayerWhenToTurnAngle());
	}
	else if(WM->getSecondClosestInSetTo(OBJECT_SET_TEAMMATES, OBJECT_BALL)
	  == WM->getAgentObjectType())
	{
	    soc = moveToPos(pointKickToK(), PS->getPlayerWhenToTurnAngle());
	}
	else
	{
	    soc = moveToPos(WM->getStrategicPosition(), PS->getPlayerWhenToTurnAngle());
	}

	ACT->putCommandInQueue( soc ); // �����������
        ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    

    else  /// ���else ���Ӧ�ÿ���֪�����ǿ����Ժ�ľ��ߣ� Ҳ��������Ҫ�ı�������ģʽ�µľ��ߣ� ����Ҫ�����Ǿʹ����￪ʼ�������� Start��
    {
        formations->setFormation( FT_334_OFFENSIVE );//������ӽ�������
        soc.commandType = CMD_ILLEGAL;//��ʼ��soc�������


        if ( WM->getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )//�ж϶���Ŀ��Ŷȣ����С��ĳ����ֵ����...Ҳ����˵������ܶ�����û�п�������������
        {
            ACT->putCommandInQueue( soc = searchBall() );   // if ball pos unknown //ִ�����������������������
            ACT->putCommandInQueue( alignNeckWithBody( ) ); // search for it //ͬʱ�Ѳ���������һ��ת
        }
        else if ( WM->isBallKickable())                   // if kickable // �������֪�����ҵ�ǰ�����ҽ���(����)
        {   
	    if( WM->isFreeKickUs() )//��������
	    {
	      	double ang = (VecPosition(52.5,0)-posAgent).getDirection();
		if ( WM->isOpponentAtAngleEx(ang-45, ang, 6) ) 
		    ang+=45; 
		else if ( WM->isOpponentAtAngleEx(ang,ang+45,6) ) 
		    ang-=45; 
		soc = kickTo ( ang , SS->getBallSpeedMax()  );  
	    }
	    
	    else if ( WM->getBallPos().getX() < -25 )//�ڱ����볡
	    {
		if( WM->getPlayerNumber() == 2 || WM->getPlayerNumber() == 5 )
		{
		    soc = clearBall( CLEAR_BALL_DEFENSIVE );
		}
		
		if( WM->getPlayerNumber() == 6 || WM->getPlayerNumber() == 7 )
		{
		    soc = clearBall( CLEAR_BALL_DEFENSIVE );
		}
		
		if( WM->getPlayerNumber() == 10 || WM->getPlayerNumber() == 11 )
		{
		    if( WM->getPlayerNumber() == 10 )
			soc = directPass( VecPosition(10, 25), PASS_FAST);
		    else
			soc = directPass( VecPosition(10, -25), PASS_FAST);			
		}
		
		else
		{
		    if( posAgent.getY() > 0) 
			soc = directPass( VecPosition( posAgent.getX() + 20, 25), PASS_FAST);
		    else
		      	soc = directPass( VecPosition( posAgent.getX() + 20, -25), PASS_FAST);
		}
		    
	    }
	    
	    else if ( WM->getBallPos().getX() > -25 && WM->getBallPos().getX() < -5 )//�ڱ����볡
	    {
		if( WM->getPlayerNumber() == 2 || WM->getPlayerNumber() == 5 )
		{
		    if( WM->getPlayerNumber() == 2 )
			soc = directPass( VecPosition(5, 25), PASS_FAST);
		    else
			soc = directPass( VecPosition(5, -25), PASS_FAST);	
		    soc = clearBall( CLEAR_BALL_DEFENSIVE );
		}
		
		if( WM->getPlayerNumber() == 6 || WM->getPlayerNumber() == 7 )
		{
		    if( WM->getPlayerNumber() == 7 )
			soc = directPass( VecPosition(15, 25), PASS_FAST);
		    else
			soc = directPass( VecPosition(15, -25), PASS_FAST);
		    soc = clearBall( CLEAR_BALL_DEFENSIVE );
		}
		
		if( WM->getPlayerNumber() == 10 || WM->getPlayerNumber() == 11 )
		{
		    if( WM->getPlayerNumber() == 10 )
			soc = directPass( VecPosition(20, 30), PASS_FAST);
		    else
			soc = directPass( VecPosition(20, -30), PASS_FAST);			
		}
		
		else
		{
		    if( posAgent.getY() > 0) 
			soc = directPass( VecPosition( posAgent.getX() + 20, 25), PASS_FAST);
		    else
		      	soc = directPass( VecPosition( posAgent.getX() + 20, -25), PASS_FAST);
		}
		    
	    }
	    
	    else if ( WM->getBallPos().getX() < 10 && WM->getBallPos().getX() >= -5 )//�ڱ����볡
	    {

		if( WM->getPlayerNumber() == 10 || WM->getPlayerNumber() == 11 )
		{
		    if( WM->getPlayerNumber() == 10 )
			soc = dribble_Dodge( VecPosition(35, 30), posAgent);
		    else
			soc = dribble_Dodge( VecPosition(35, -30), posAgent);			
		}
		
		else
		{
		    if(WM->getBallPos().getY() < 0)
			soc = leadingPass( OBJECT_TEAMMATE_11 , 1.5 );
		    else
			soc = leadingPass( OBJECT_TEAMMATE_10 , 1.5 );	
		}
		    
	    }
	     
	    else if( WM->getBallPos().getX() >= 10 && WM->getBallPos().getX() <= 45)//�����볡
	    {
		 if( WM->getPlayerNumber() == 11 )//��·ѡ��11
		 {
		    Circle cir1( posAgent, 6 );
		    int num=WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);
		    
		    if(num >= 2 && !WM->isInTheirPenaltyArea( WM->getBallPos()) )//����������û������
		    {	
			ObjectT temp = OBJECT_TEAMMATE_8;
			
			if( WM->getGlobalPosition(temp).getX() > 
			  WM->getGlobalPosition(OBJECT_TEAMMATE_6).getX() &&
			    WM->getGlobalPosition( temp ).getX() > 
			  posAgent.getX() - 10 ) //8�ű�6��ǰ,�Ҳ�Զ
			{
			    soc = throughPass( temp , posGoal );
			}
			
			else if(posAgent.getDistanceTo( WM->getClosestInSetTo(OBJECT_SET_TEAMMATES, posAgent) ) 
			  < posAgent.getDistanceTo( WM->getGlobalPosition(OBJECT_TEAMMATE_6)) )
			{
			    soc = leadingPass( WM->getClosestInSetTo(OBJECT_SET_TEAMMATES , posAgent) ,1 );			      
			}
			
			else if( WM->getGlobalPosition( OBJECT_TEAMMATE_6 ).getX() > 
			  posAgent.getX() - 10 ) //6�����butaiԶ
			{
			    soc = leadingPass( OBJECT_TEAMMATE_6 ,1 );
			}
			
			else
			    soc = maxAngShoot1(posAgent);
			
		    }
		    
		    else if( WM->isInTheirPenaltyArea( WM->getBallPos()) && fabs( WM->getBallPos().getY()) < 13 )//�����������
		    {
			double ang = (VecPosition(52.5,0)-posAgent).getDirection();
			if( WM-> isOpponentAtAngleEx(ang-15, ang + 15, 15) )
			{
			    soc = leadingPass( WM->getClosestInSetTo( OBJECT_SET_TEAMMATES , posAgent ), 1);			  
			}

			else
			    soc = maxAngShoot( posAgent );
		    }
		    
		    else//��������λ�ƶ�
		    {
			VecPosition posGoal = WM->getPosOpponentGoal();
			VecPosition atkPos( posGoal.getX() - 6, posGoal.getY() - 15 );//������λ
			
			if( WM->getPlayerNumber() == 10 )
			{
			    atkPos.setY( posGoal.getY() + 15 ); 
			}
			
			soc = dribble_Dodge( atkPos , posAgent );
			
		    }
		 }
		 
		 else if( WM->getPlayerNumber() == 10 )//��·ѡ��10
		 {
		    Circle cir1( posAgent, 6 );
		    int num=WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);
		    
		    if(num >= 2 && !WM->isInTheirPenaltyArea( WM->getBallPos()) )//����������û������
		    {	
			ObjectT temp = OBJECT_TEAMMATE_9;
			
			if( WM->getGlobalPosition(temp).getX() > 
			  WM->getGlobalPosition(OBJECT_TEAMMATE_7).getX() &&
			    WM->getGlobalPosition( temp ).getX() > 
			  posAgent.getX() - 10 ) //9�ű�7��ǰ,�Ҳ�Զ
			{
			    soc = throughPass( temp , posGoal );
			}
			
			else if(posAgent.getDistanceTo( WM->getClosestInSetTo(OBJECT_SET_TEAMMATES, posAgent) ) 
			  < posAgent.getDistanceTo( WM->getGlobalPosition(OBJECT_TEAMMATE_7)) )
			{
			    soc = leadingPass( WM->getClosestInSetTo(OBJECT_SET_TEAMMATES, posAgent) ,1 );			      
			}
			
			else if( WM->getGlobalPosition( OBJECT_TEAMMATE_7 ).getX() > 
			  posAgent.getX() - 10 ) //7����ò�̫Զ
			{
			    soc = leadingPass( OBJECT_TEAMMATE_7 ,1 );
			}
			
			else
			    soc = maxAngShoot1(posAgent);
		    }
 		    
		    else if( WM->isInTheirPenaltyArea( WM->getBallPos()) && fabs( WM->getBallPos().getY()) < 13 )//�����������
		    {
			double ang = (VecPosition(52.5,0)-posAgent).getDirection();
			if( WM-> isOpponentAtAngleEx(ang-15, ang + 15, 15) )
			{
			    soc = leadingPass( WM->getClosestInSetTo( OBJECT_SET_TEAMMATES , posAgent ), 1);				  
			}

			else
			    soc = maxAngShoot( posAgent );
		    }
		    else//��������λ�ƶ�
		    {
			VecPosition posGoal = WM->getPosOpponentGoal();
			VecPosition atkPos( posGoal.getX() - 6, posGoal.getY() - 15 );//������λ
			
			if( WM->getPlayerNumber() == 10 )
			{
			    atkPos.setY( posGoal.getY() + 15 ); 
			}
			
			soc = dribble_Dodge( atkPos , posAgent );			
		    }
		 }
		 
		 else if( WM->getPlayerNumber() == 6 )//��Ӧѡ��6
		 {
		    Circle cir1( posAgent, 6 );
		    int num = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);		    
		    double ang = (VecPosition(52.5,0)-posAgent).getDirection();
		    
		    
		    if(num >= 2 &&  WM-> isOpponentAtAngleEx(ang-20, ang + 20, 15))
		    {			
			ObjectT temp = OBJECT_TEAMMATE_9;
			
			Circle cir1( WM->getGlobalPosition(temp), 6 );
			int num1 = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);		    

			Circle cir2( WM->getGlobalPosition(OBJECT_TEAMMATE_8), 6 );
			int num2 = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);	
			
			if( num1 > num2  )
			    soc = leadingPass( OBJECT_TEAMMATE_8 , 1);
			else if( num1 < num2)
			    soc = leadingPass( temp , 1);
			else
			    soc = leadingPass( OBJECT_TEAMMATE_11 ,1);
		    }

		    else if( WM->getBallPos().getX() < 35 )
		    {
			soc = dribble_Dodge( VecPosition(35, -10) , posAgent );			
		    }
		    
		    else
			soc = maxAngShoot( posAgent );//ֱ������		 
		}
		 
		 else if( WM->getPlayerNumber() == 7 )//��Ӧѡ��7
		 {
		    Circle cir1( posAgent, 6 );
		    int num = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);		    
		    double ang = (VecPosition(52.5,0)-posAgent).getDirection();
			
		    if(num >= 2 &&  WM-> isOpponentAtAngleEx(ang-20, ang + 20, 15))
		    {			
			ObjectT temp = OBJECT_TEAMMATE_9;
			
			Circle cir1( WM->getGlobalPosition(temp), 6 );
			int num1 = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);		    

			Circle cir2( WM->getGlobalPosition(OBJECT_TEAMMATE_8), 6 );
			int num2 = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);	
			
			if( num1 > num2  )
			    soc = leadingPass( OBJECT_TEAMMATE_8 , 1);
			else if( num1 < num2 )
			    soc = leadingPass( temp , 1);
			
			else 
			    soc = leadingPass( OBJECT_TEAMMATE_10 ,1);
	    
		    }

		    else if( WM->getBallPos().getX() < 35 )
		    {
			soc = dribble_Dodge( VecPosition(35, 9) , posAgent );			
		    }
		    
		    else
			soc = maxAngShoot( posAgent );//ֱ������		 
		}
		
		 else if( WM->getPlayerNumber() == 9 || WM->getPlayerNumber() == 8 )//ͻ����8,9
		 {
		    Circle cir1( posAgent, 6 );
		    int num = WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);	
		    
		    double ang = (VecPosition(52.5,0)-posAgent).getDirection();
		    
		    if( WM->isOpponentAtAngleEx( ang-15, ang+15, 15) //����Ӧ�������� 5.14
		      && WM->getBallPos().getX() > 33
		      && fabs( WM->getBallPos().getY() ) < 16)
		    {
			soc = maxAngShoot1( posAgent );  
		    }
		    
		    else if(num >= 3)
		    {
		        if(posAgent.getDistanceTo( WM->getGlobalPosition(OBJECT_TEAMMATE_6)) //���������ش�
			  < posAgent.getDistanceTo( WM->getGlobalPosition(OBJECT_TEAMMATE_7)) )
			    soc = leadingPass( OBJECT_TEAMMATE_6 ,1 );
			else
			    soc = leadingPass( OBJECT_TEAMMATE_7 ,1 );
		    }
		    
		    else if( WM->getBallPos().getX() < 30 )
			soc = dribble_Dodge( WM->getPosOpponentGoal() , posAgent );
		    else
			soc = maxAngShoot( posAgent );//ֱ������	
		 }
		 
		 else soc = maxAngShoot1( posAgent );		
	    }

	    else if( WM->getBallPos().getX() >= 45 &&
		     WM->getBallPos().getX() < 52)//��������űȽϽ��� 
	    {
		 if( WM->getPlayerNumber() == 9 || WM->getPlayerNumber() == 8 )//��Ҫͻ��ѡ��
		 {
		    Circle cir1( posAgent, 6 );
		    int num=WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);
		    double ang = (VecPosition(52.5,0)-posAgent).getDirection();
		    
		    if( WM->isOpponentAtAngleEx( ang-15, ang+15, 15))
		    {
		        soc = directPass( WM->getGlobalPosition( 
		        WM->getClosestInSetTo(OBJECT_SET_TEAMMATES, posAgent) ), PASS_FAST);
		    }
		    else soc = maxAngShoot( posAgent );
		 }
		 
		 else if( WM->getPlayerNumber() == 10 )
		 {
		    Circle cir1( posAgent, 3 );
		    int num=WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);		    
		    double ang = (VecPosition(52.5,0)-posAgent).getDirection();
		    if( WM-> isOpponentAtAngleEx(ang-15, ang +15, 15) )
		    {
			    soc = leadingPass( OBJECT_TEAMMATE_8 , 1);			
		    }
		    if(num >= 1)
		    {
		        if(WM->getGlobalPosition(OBJECT_TEAMMATE_7).getX() 
			  < WM->getGlobalPosition(OBJECT_TEAMMATE_9).getX() )
			    soc = directPass( WM->getGlobalPosition( OBJECT_TEAMMATE_9 ), PASS_FAST);	
			else
			    soc = directPass( WM->getGlobalPosition( OBJECT_TEAMMATE_7 ), PASS_FAST);	
		    }
		    else soc = maxAngShoot1( posAgent );
		 }
		 
		 else if( WM->getPlayerNumber() == 11 )
		 {
		    Circle cir1( posAgent, 3 );
		    int num=WM->getNrInSetInCircle(OBJECT_SET_OPPONENTS,cir1);		    
		    double ang = (VecPosition(52.5,0)-posAgent).getDirection();
		    
		    if( WM-> isOpponentAtAngleEx(ang-15, ang + 15, 15) )
		    {
			    soc = leadingPass( OBJECT_TEAMMATE_9 , 1);			
		    }
		    else if(num >= 1)
		    {
		        if(WM->getGlobalPosition(OBJECT_TEAMMATE_6).getX() 
			  < WM->getGlobalPosition(OBJECT_TEAMMATE_8).getX() )
			    soc = directPass( WM->getGlobalPosition( OBJECT_TEAMMATE_8 ), PASS_FAST);	
			else
			    soc = directPass( WM->getGlobalPosition( OBJECT_TEAMMATE_6 ), PASS_FAST);	
		    }
		    else soc = maxAngShoot( posAgent );
		 }
		 
		 else soc = maxAngShoot1( posAgent );
	    }

	    else//Ĭ�ϲ���
	    {
		VecPosition posGoal(PITCH_LENGTH/2.0, 
				       (-1 + 2*(WM->getCurrentCycle()%2)) * 
				       0.4 * SS->getGoalWidth());
		soc=kickTo( posGoal,SS->getBallSpeedMax());// �����ŷ��������������
	    }

            ACT->putCommandInQueue( soc ); // �����������
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) ); // �Ѳ���ת����Ҳ����һֱ������
            Log.log( 100, "kick ball" );
        }

        else if ( WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp )
                  == WM->getAgentObjectType()  && !WM->isDeadBallThem() ) // ��������ҵĿ��Ʒ�Χ�£����ǵ�ǰ���������������ң����Ҿ�ȥִ��������
        {                                                // if fastest to ball
            Log.log( 100, "I am fastest to ball; can get there in %d cycles", iTmp );
            soc = intercept( false );                      // intercept the ball
	    if ( WM->getPlayerNumber() == 2 || WM->getPlayerNumber() == 3 || WM->getPlayerNumber() == 5 )
	    {
	        if ( soc.commandType == CMD_DASH &&            // if stamina low  // �����Ƕ�������һ���������������;Ͱ�Dash��dPower��С ��������
                    WM->getAgentStamina().getStamina() <
                    SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
	      {
                soc.dPower = 32.0 * WM->getAgentStamina().getRecovery(); // dash slow
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	      }
	      else                                           // if stamina high
	      {
                ACT->putCommandInQueue( soc );               // dash as intended
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	      }
	      
	    }
	    else if ( WM->getPlayerNumber() == 10 || WM->getPlayerNumber() == 11 )
	    {
	        if ( soc.commandType == CMD_DASH &&            // if stamina low  // �����Ƕ�������һ���������������;Ͱ�Dash��dPower��С ��������
                    WM->getAgentStamina().getStamina() <
                    SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
	      {
                soc.dPower = 32.0 * WM->getAgentStamina().getRecovery(); // dash slow
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	      }
	      else                                           // if stamina high
	      {
                ACT->putCommandInQueue( soc );               // dash as intended
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	      }
	      
	    }
	    else
	    {
	       if ( soc.commandType == CMD_DASH &&            // if stamina low  // �����Ƕ�������һ���������������;Ͱ�Dash��dPower��С ��������
                    WM->getAgentStamina().getStamina() <
                    SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
	      {
                soc.dPower = 32.0 * WM->getAgentStamina().getRecovery(); // dash slow
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	      }
	      else                                           // if stamina high
	      {
                ACT->putCommandInQueue( soc );               // dash as intended
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
	      }
	    }

        }
        else if(WM->getSecondClosestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL,(double*) &iTmp ) 
	  == WM->getAgentObjectType()  && 
	  WM->isDeadBallThem()&&( WM->getBallPos().getDistanceTo(WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp ))>SS->getMaximalKickDist()))
	{
	  
            soc = intercept( false );                      // intercept the ball
            if ( soc.commandType == CMD_DASH &&            // if stamina low  // �����Ƕ�������һ���������������;Ͱ�Dash��dPower��С ��������
                    WM->getAgentStamina().getStamina() <
                    SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
            {
                soc.dPower = 30.0 * WM->getAgentStamina().getRecovery(); // dash slow
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
            else                                           // if stamina high
            {
                ACT->putCommandInQueue( soc );               // dash as intended
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
	}
	
        else if ( posAgent.getDistanceTo(WM->getStrategicPosition()) >
                  1.5 + fabs(posAgent.getX()-posBall.getX())/10.0) // ������������������������Զһ������ˣ�������Լ������ε�̫Զ�����ܻ� �Լ������ε�ȥ��
            // if not near strategic pos
        {
            if ( WM->getAgentStamina().getStamina() >    // if stamina high
                    SS->getRecoverDecThr()*SS->getStaminaMax()+800 )
            {
                soc = moveToPos(WM->getStrategicPosition(),
                                PS->getPlayerWhenToTurnAngle());
                ACT->putCommandInQueue( soc );            // move to strategic pos
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
            else                                        // else watch ball
            {
                ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
        }
        else if ( fabs( WM->getRelativeAngle( OBJECT_BALL ) ) > 1.0 ) // watch ball //����ʣ�µ���Ա�أ��Ϳ��򡣣�
        {///������������Ա����λ����

            ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else                                         // nothing to do
            ACT->putCommandInQueue( SoccerCommand(CMD_TURNNECK,0.0) );
    }
    return soc;
}

/*!This method is a simple goalie based on the goalie of the simple Team of
   FC Portugal. It defines a rectangle in its penalty area and moves to the
   position on this rectangle where the ball intersects if you make a line
   between the ball position and the center of the goal. If the ball can
   be intercepted in the own penalty area the ball is intercepted and catched.
*/
SoccerCommand Player::deMeer5_goalie(  )
{
    int i;
    SoccerCommand soc;
    VecPosition   posAgent = WM->getAgentGlobalPosition();
    AngDeg        angBody  = WM->getAgentGlobalBodyAngle();

    // define the top and bottom position of a rectangle in which keeper moves
    static const VecPosition posLeftTop( -PITCH_LENGTH/2.0 +
                                         0.7*PENALTY_AREA_LENGTH, -19.0 );
    static const VecPosition posRightTop( -PITCH_LENGTH/2.0 +
                                          0.7*PENALTY_AREA_LENGTH, +19.0 );

    // define the borders of this rectangle using the two points.
    static Line  lineFront = Line::makeLineFromTwoPoints(posLeftTop,posRightTop);
    static Line  lineLeft  = Line::makeLineFromTwoPoints(
                                 VecPosition( -50.0, posLeftTop.getY()), posLeftTop );
    static Line  lineRight = Line::makeLineFromTwoPoints(
                                 VecPosition( -50.0, posRightTop.getY()),posRightTop );
    
    if(WM->isFreeKickThem())
    {
                  VecPosition posMyGoal = ( WM->getSide() == SIDE_LEFT )
                                    ? SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_L, SIDE_LEFT )
                                    : SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_R, SIDE_RIGHT);
         Line lineBall = Line::makeLineFromTwoPoints( WM->getBallPos(),posMyGoal);
                 VecPosition posIntersect = lineFront.getIntersection( lineBall );

            // outside rectangle, use line at side to get intersection
            if (posIntersect.isRightOf( posRightTop ) )
                posIntersect = lineRight.getIntersection( lineBall );
            else if (posIntersect.isLeftOf( posLeftTop )  )
                posIntersect = lineLeft.getIntersection( lineBall );

            if ( posIntersect.getX() < -49.0 )
                posIntersect.setX( -49.0 );

     soc = moveToPos((52,3), PS->getPlayerWhenToTurnAngle());
      ACT->putCommandInQueue( soc );
      ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }

    if ( WM->isBeforeKickOff( ) )
    {
        if ( formations->getFormation() != FT_INITIAL || // not in kickoff formation
                posAgent.getDistanceTo( WM->getStrategicPosition() ) > 2.0 )
        {
            formations->setFormation( FT_INITIAL );       // go to kick_off formation
            ACT->putCommandInQueue( soc=teleportToPos(WM->getStrategicPosition()) );
        }
        else                                            // else turn to center
        {
            ACT->putCommandInQueue( soc = turnBodyToPoint( VecPosition( 0, 0 ), 0 ));
            ACT->putCommandInQueue( alignNeckWithBody( ) );
        }
        return soc;
    }


    if ( WM->getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )
    {                                                // confidence ball too  low
        ACT->putCommandInQueue( searchBall() );        // search ball
        ACT->putCommandInQueue( alignNeckWithBody( ) );
    }
    else if ( WM->getPlayMode() == PM_PLAY_ON || WM->isFreeKickThem() ||
              WM->isCornerKickThem() )
    {
        if ( WM->isBallCatchable() )
        {
            ACT->putCommandInQueue( soc = catchBall() );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else if ( WM->isBallKickable() )
        {
            soc = kickTo( WM->getSecondClosestInSetTo(OBJECT_SET_TEAMMATES,OBJECT_TEAMMATE_1), 2.0 );
            ACT->putCommandInQueue( soc );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else if ( WM->isInOwnPenaltyArea( getInterceptionPointBall( &i, true ) ) &&
                  WM->getFastestInSetTo( OBJECT_SET_PLAYERS, OBJECT_BALL, &i ) ==
                  WM->getAgentObjectType() )
        {
            ACT->putCommandInQueue( soc = intercept( true ) );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else
        {
            // make line between own goal and the ball
            VecPosition posMyGoal = ( WM->getSide() == SIDE_LEFT )
                                    ? SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_L, SIDE_LEFT )
                                    : SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_R, SIDE_RIGHT);
            Line lineBall = Line::makeLineFromTwoPoints( WM->getBallPos(),posMyGoal);

            // determine where your front line intersects with the line from ball
            VecPosition posIntersect = lineFront.getIntersection( lineBall );

            // outside rectangle, use line at side to get intersection
            if (posIntersect.isRightOf( posRightTop ) )
                posIntersect = lineRight.getIntersection( lineBall );
            else if (posIntersect.isLeftOf( posLeftTop )  )
                posIntersect = lineLeft.getIntersection( lineBall );

            if ( posIntersect.getX() < -49.0 )
                posIntersect.setX( -49.0 );

            // and move to this position
            if ( posIntersect.getDistanceTo( WM->getAgentGlobalPosition() ) > 0.5 )
            {
                soc = moveToPos( posIntersect, PS->getPlayerWhenToTurnAngle() );
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
            else
            {
                ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
        }
    }
    
    else if ( WM->isFreeKickUs() == true || WM->isGoalKickUs() == true )
    {
        if ( WM->isBallKickable() )
        {
            if ( WM->getTimeSinceLastCatch() == 25 && WM->isFreeKickUs() )
            {
                // move to position with lesser opponents.
                if ( WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
                                             Circle(posRightTop, 15.0 )) <
                        WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
                                                Circle(posLeftTop,  15.0 )) )
                    soc.makeCommand( CMD_MOVE,posRightTop.getX(),posRightTop.getY(),0.0);
                else
                    soc.makeCommand( CMD_MOVE,posLeftTop.getX(), posLeftTop.getY(), 0.0);
                ACT->putCommandInQueue( soc );
            }
            else if ( WM->getTimeSinceLastCatch() > 28 )
            {
                soc = kickTo( WM->getSecondClosestInSetTo(OBJECT_SET_TEAMMATES,OBJECT_TEAMMATE_1), 2.0 );
                ACT->putCommandInQueue( soc );
            }
        }
        else if ( WM->isGoalKickUs()  )
        {
            ACT->putCommandInQueue( soc = intercept( true ) );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    else
    {
        ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
        ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    return soc;
}