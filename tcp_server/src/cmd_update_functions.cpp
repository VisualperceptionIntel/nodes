#include "server_node.h"



bool IsValidInt(char* requested_value)
{
	bool Checked = true;
	int i = 0;
	do
	{
		//valid digit?
		if (isdigit(requested_value[i]))
		{
			//to the next character
			i++;
			Checked = true;
		}
		else
		{
			//to the next character
			i++;
			Checked = false;
			break;
		}
	} while (requested_value[i] != '\0');

	return Checked;
}

//int validity_check( int type , char* update_value)
//{
//if ( update_value != NULL && strcmp(update_value,"(NULL)")!=0 && strcmp(update_value,"(null)")!=0 && strcmp(update_value,"NULL")!=0  )
//            {
//                return 1;
//            }
//else
//return 0;
//}

int base_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"base_busy")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0  )
            {
                command_update.is_base_busy=atoi(str_tok_buf_tmp[3].tok);
                is_updated[IS_BASE_BUSY] = 1;


            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"pos_x")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0  )
            {
                command_update.base_position_x=atof(str_tok_buf_tmp[5].tok);
                is_updated[BASE_POSITION_X] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"pos_y")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0  )
            {
                command_update.base_position_y=atof(str_tok_buf_tmp[7].tok);
                is_updated[BASE_POSITION_Y] = 1;
            }
        }
        /*if( strcmp(str_tok_buf_tmp[8].tok,"light_state")==0 ){
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0 ){
                command_update.light_status=atoi(str_tok_buf_tmp[9].tok);
                is_updated[LIGHT_STATUS] = 1;
            }
        }*/
        if( strcmp(str_tok_buf_tmp[8].tok,"speech_request")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0  )
            {
                command_update.speech_request=atoi(str_tok_buf_tmp[9].tok);
                is_updated[SPEECH_REQUEST] = 1;
            }
        }

        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }

    update_master_commands( command_update, is_updated );

    return 1;
}

int arms_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"left_busy")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0   )
            {
                command_update.is_left_arm_busy=atoi(str_tok_buf_tmp[3].tok);
                is_updated[IS_LEFT_ARM_BUSY] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"right_busy")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0  )
            {
                command_update.is_right_arm_busy=atoi(str_tok_buf_tmp[5].tok);
                is_updated[IS_RIGHT_ARM_BUSY] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"arm_actions")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0  )
            {
                command_update.arm_actions=str_tok_buf_tmp[7].tok;

                if (strcmp(str_tok_buf_tmp[7].tok,"walk")==0)
                {
                    command_update.left_arm_move=WALK;
                    command_update.right_arm_move=WALK;
                }
                else if (strcmp(str_tok_buf_tmp[7].tok,"pick")==0)
                {
                    command_update.left_arm_move=PICK;
                    command_update.right_arm_move=PICK;
                }
                else if (strcmp(str_tok_buf_tmp[7].tok,"place")==0)
                {
                    command_update.left_arm_move=PLACE;
                    command_update.right_arm_move=PLACE;
                }
                else if (strcmp(str_tok_buf_tmp[7].tok,"wave")==0)
                {
                    command_update.left_arm_move=WAVE;
                    command_update.right_arm_move=WAVE;
                }
                else  //home
                {
                    command_update.left_arm_move=HOME;
                    command_update.right_arm_move=HOME;
                }


                is_updated[ARM_ACTIONS] = 1;
                is_updated[LEFT_ARM_MOVE] = 1;
                is_updated[RIGHT_ARM_MOVE] = 1;
            }
        }
        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }

    update_master_commands( command_update, is_updated );

    return 1;
}

int detect_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};
    printf("%s,%s,%s,%s,%s,%s,%s,%s,%s",str_tok_buf_tmp[2].tok,str_tok_buf_tmp[3].tok,str_tok_buf_tmp[4].tok,str_tok_buf_tmp[5].tok,str_tok_buf_tmp[6].tok,str_tok_buf_tmp[7].tok,str_tok_buf_tmp[8].tok,str_tok_buf_tmp[9].tok,str_tok_buf_tmp[10].tok);
    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"emotion")==0 )
        {
            if ( IsValidInt(str_tok_buf_tmp[3].tok) == true)
            {
                printf("valid int \n");
                command_update.emotion_status=atoi(str_tok_buf_tmp[3].tok);
                is_updated[EMOTION_STATUS] = 1;
                  printf("valid int saved \n");
            }
            if( strcmp(str_tok_buf_tmp[4].tok,"<EOC>")==0 )
            {
                printf("==================About to update table===================");
                update_master_commands( command_update, is_updated );
                return 1;
            }

        }
        if( strcmp(str_tok_buf_tmp[4].tok,"cog_status")==0 )
        {
            if ( IsValidInt(str_tok_buf_tmp[5].tok) == true)
            {
                command_update.face_cognition_status=atoi(str_tok_buf_tmp[5].tok);
                is_updated[FACE_COGNITION_STATUS] = 1;

          }
        }

        if( strcmp(str_tok_buf_tmp[6].tok,"cog_outcome")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0 )
            {
                command_update.face_cognition_outcome=str_tok_buf_tmp[7].tok;
                is_updated[FACE_COGNITION_OUTCOME] = 1;

                str_tok_buffer sql_visit_tok_buf = {NULL};
                printf("visit data retrieving here \n");
                if(command_update.face_cognition_outcome!=master_commands.face_cognition_outcome && strcmp(command_update.face_cognition_outcome,"NULL" ) !=0 && strcmp(master_commands.id_card_name,"NULL" ) == 0)
                {
                    mysql_visit_update(sql_visit_tok_buf,is_updated,command_update ,2,master_commands );//1-scanner 2-cognition
                    printf("visit data retrieved");

                 }
                 else
                 {
                         if(command_update.face_cognition_outcome=="NULL" && master_commands.speech_status == 0)
                         {
                        command_update.print_status=0;
                        is_updated[PRINT_STATUS] = 1;
                        command_update.visitee_name= new char;
                        strcpy(command_update.visitee_name,"NULL");
                        command_update.appointment_date= new char;
                        strcpy(command_update.appointment_date,"NULL");
                        command_update.appointment_time= new char;
                        strcpy(command_update.appointment_time,"NULL");
                        command_update.appointment_location= new char;
                        strcpy(command_update.appointment_location,"NULL");
                        is_updated[VISITEE_NAME] = 1;
                        is_updated[APPOINTMENT_DATE] = 1;
                        is_updated[APPOINTMENT_TIME] = 1;
                        is_updated[APPOINTMENT_LOCATION] = 1;
                        }
                 }


            }

        }

        if( strcmp(str_tok_buf_tmp[8].tok,"face_detect")==0 )
        {
            if ( IsValidInt(str_tok_buf_tmp[9].tok)== true )
            {
                command_update.face_detect=atoi(str_tok_buf_tmp[9].tok);
                is_updated[FACE_DETECT] = 1;
            }
        }

        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }
    printf("==================About to update table===================");
    update_master_commands( command_update, is_updated );

    return 1;
}

int scanner_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

/*if( strcmp(str_tok_buf_tmp[6].tok,"cog_outcome")==0 ){
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"NULL")!=0 ){
                command_update.face_cognition_outcome=str_tok_buf_tmp[7].tok;
                is_updated[FACE_COGNITION_OUTCOME] = 1;

                str_tok_buffer sql_visit_tok_buf = {NULL};
                printf("visit data retrieving here \n");
                if(command_update.face_cognition_outcome!=master_commands.face_cognition_outcome && command_update.face_cognition_outcome!="NULL")
                {
                    mysql_visit_update(sql_visit_tok_buf,is_updated,command_update ,2,master_commands );//1-scanner 2-cognition
                    printf("visit data retrieved");

                 }else{
                         if(command_update.face_cognition_outcome=="NULL" && master_commands.speech_status == 0)
                         {
                        command_update.print_status=0;
                        is_updated[PRINT_STATUS] = 1;
                        command_update.visitee_name= new char;
                        strcpy(command_update.visitee_name,"NULL");
                        command_update.appointment_date= new char;
                        strcpy(command_update.appointment_date,"NULL");
                        command_update.appointment_time= new char;
                        strcpy(command_update.appointment_time,"NULL");
                        command_update.appointment_location= new char;
                        strcpy(command_update.appointment_location,"NULL");
                        is_updated[VISITEE_NAME] = 1;
                        is_updated[APPOINTMENT_DATE] = 1;
                        is_updated[APPOINTMENT_TIME] = 1;
                        is_updated[APPOINTMENT_LOCATION] = 1;
                        }
                 }}}*/

        if( strcmp(str_tok_buf_tmp[2].tok,"id_card_name")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.id_card_name=str_tok_buf_tmp[3].tok;

                //log_string=log_string+","+str_tok_buf_tmp[2].tok+":"+str_tok_buf_tmp[3].tok;

                is_updated[ID_CARD_NAME] = 1;

                str_tok_buffer sql_visit_tok_buf = {NULL};
                printf("visit data retrieving here \n");
                if(command_update.id_card_name!=master_commands.id_card_name)
                {
                    mysql_visit_update(sql_visit_tok_buf,is_updated,command_update ,1,master_commands );//1-scanner 2-cognition
                    printf("visit data retrieved");

                 }else{
                         if(command_update.id_card_name=="NULL")
                         {
                        command_update.print_status=0;
                        is_updated[PRINT_STATUS] = 1;
                        command_update.visitee_name= new char;
                        strcpy(command_update.visitee_name,"NULL");
                        command_update.appointment_date= new char;
                        strcpy(command_update.appointment_date,"NULL");
                        command_update.appointment_time= new char;
                        strcpy(command_update.appointment_time,"NULL");
                        command_update.appointment_location= new char;
                        strcpy(command_update.appointment_location,"NULL");
                        is_updated[VISITEE_NAME] = 1;
                        is_updated[APPOINTMENT_DATE] = 1;
                        is_updated[APPOINTMENT_TIME] = 1;
                        is_updated[APPOINTMENT_LOCATION] = 1;
                        }
                 }
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"card_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0)
            {
                command_update.card_reader_status=atoi(str_tok_buf_tmp[5].tok);
                is_updated[CARD_READER_STATUS] = 1;
            }
        }
        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }

    update_master_commands( command_update, is_updated );
    return 1;
}


int printer_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"print_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0  )
            {
                command_update.print_status=atoi(str_tok_buf_tmp[3].tok);
                is_updated[PRINT_STATUS] = 1;
            }
        }
//        if( strcmp(str_tok_buf_tmp[4].tok,"card_status")==0 )
//        {
//            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0  )
//            {
//                command_update.card_reader_status=atoi(str_tok_buf_tmp[5].tok);
//                is_updated[CARD_READER_STATUS] = 1;
//            }
//        }
        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }

    update_master_commands( command_update, is_updated );

    return 1;
}

int face_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{

    int ret=1;
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();
//             cout<<typeid(command_update[1]).name();
//             cout<<typeid(command_update[2]).name();
//                  cout<<typeid(command_update[3]).name();
        if( strcmp(str_tok_buf_tmp[2].tok,"forward_step")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0  )
            {
                command_update.base_forward_step=atoi(str_tok_buf_tmp[3].tok);
                is_updated[BASE_FORWARD_STEP] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"backward_step")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0   )
            {
                command_update.base_backward_step=atoi(str_tok_buf_tmp[5].tok);
                is_updated[BASE_BACKWARD_STEP] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"rotate_left")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0  )
            {
                command_update.base_rotate_left=atoi(str_tok_buf_tmp[7].tok);
                is_updated[BASE_ROTATE_LEFT] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[8].tok,"rotate_right")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[9].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0  )
            {
                command_update.base_rotate_right=atoi(str_tok_buf_tmp[9].tok);
                is_updated[BASE_ROTATE_RIGHT] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[10].tok,"light_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[11].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[11].tok,"(null)")!=0  )
            {
                command_update.light_status=atoi(str_tok_buf_tmp[11].tok);
                is_updated[LIGHT_STATUS] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[12].tok,"latitude")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[13].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[13].tok,"(null)")!=0  )
            {
                command_update.current_latitude=atof(str_tok_buf_tmp[13].tok);
                is_updated[CURRENT_LATITUDE] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[14].tok,"longitude")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[15].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[15].tok,"(null)")!=0  )
            {
                command_update.current_longitude=atof(str_tok_buf_tmp[15].tok);
                is_updated[CURRENT_LONGITUDE] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[16].tok,"altitude")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[17].tok,"NULL")!=0 && strcmp(str_tok_buf_tmp[17].tok,"(null)")!=0  )
            {
                command_update.current_altitude=atof(str_tok_buf_tmp[17].tok);
                is_updated[CURRENT_ALTITUDE] = 1;
            }
        }


//        if( strcmp(str_tok_buf_tmp[18].tok,"welcome")==0 )
//        {
//
//            ret = 2;
//
//        }

        if( strcmp(str_tok_buf_tmp[18].tok,"speech_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[19].tok,"NULL")!=0 )
            {
                command_update.speech_status=atoi(str_tok_buf_tmp[19].tok);
                is_updated[SPEECH_STATUS] = 1;
            }
            else if ( strcmp(str_tok_buf_tmp[19].tok,"(null)")==0  )
            {
            command_update.speech_status=master_commands.speech_status;
                is_updated[SPEECH_STATUS] = 1;

            }
        }
         if( strcmp(str_tok_buf_tmp[20].tok,"print_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[21].tok,"NULL")!=0 )
            {
                command_update.print_status=atoi(str_tok_buf_tmp[21].tok);
                is_updated[PRINT_STATUS] = 1;
            }
            else if ( strcmp(str_tok_buf_tmp[21].tok,"(null)")==0  )
            {
            command_update.print_status=master_commands.print_status;
                is_updated[PRINT_STATUS] = 1;

            }
        }


        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }

    update_master_commands( command_update, is_updated );

    return ret;
}

int obj_detect_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"frame")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0  )
            {
                command_update.obj_frame=str_tok_buf_tmp[3].tok;
                is_updated[OBJ_FRAME] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"obj_x")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0  )
            {
                command_update.obj_x=atof(str_tok_buf_tmp[5].tok);
                is_updated[OBJ_X] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"obj_y")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0  )
            {
                command_update.obj_y=atof(str_tok_buf_tmp[7].tok);
                is_updated[OBJ_Y] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[8].tok,"obj_z")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0  )
            {
                command_update.obj_z=atof(str_tok_buf_tmp[9].tok);
                is_updated[OBJ_Z] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[10].tok,"obj_height")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[11].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[11].tok,"(null)")!=0  )
            {
                command_update.obj_height=atof(str_tok_buf_tmp[11].tok);
                is_updated[OBJ_HEIGHT] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[12].tok,"obj_width")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[13].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[13].tok,"(null)")!=0  )
            {
                command_update.obj_width=atof(str_tok_buf_tmp[13].tok);
                is_updated[OBJ_WIDTH] = 1;
            }
        }
        time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }

    update_master_commands( command_update, is_updated );

    return 1;
}


int lights_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{


char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"battery")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0  )
            {
                command_update.battery_level=atoi(str_tok_buf_tmp[3].tok);
                is_updated[BATTERY_LEVEL] = 1;
            }
        }
         time_stamp(command_update.updated);
    }
    else
    {
        return -1;
    }
    update_master_commands( command_update, is_updated );
    return 1;
}


void update_mysql_cmd_unit(str_tok_buffer str_tok_buf_tmp, string query_type,char is_updated[COMMAND_UNIT_LENGTH],struct command_unit& mysql_commands)
{
    if (query_type=="visit")
    {
    //reset print_status to 0 before checking for appointment
        mysql_commands.print_status = PRINT_STATUS_NO_APPOINTMENT;
        is_updated[PRINT_STATUS] = 1;
        if( (str_tok_buf_tmp[DB_HOST].tok!="(NULL)") || (str_tok_buf_tmp[DB_HOST].tok!="(null)") || (str_tok_buf_tmp[DB_HOST].tok!="NULL") )
        {
            delete[] mysql_commands.visitee_name;
            mysql_commands.visitee_name = new char[strlen(str_tok_buf_tmp[DB_HOST].tok)+1];
            strcpy(mysql_commands.visitee_name,str_tok_buf_tmp[DB_HOST].tok);
            is_updated[VISITEE_NAME] = 1;


            if( (str_tok_buf_tmp[DB_CHECKIN_STATUS].tok!="(NULL)") || (str_tok_buf_tmp[DB_CHECKIN_STATUS].tok!="(null)")|| (str_tok_buf_tmp[DB_CHECKIN_STATUS].tok!="NULL") )
            {
                delete[] check_in;
                check_in = new char[strlen(str_tok_buf_tmp[DB_CHECKIN_STATUS].tok)+1];
                strcpy(check_in,str_tok_buf_tmp[DB_CHECKIN_STATUS].tok);
                if(atoi(str_tok_buf_tmp[DB_CHECKIN_STATUS].tok)==1)
                    mysql_commands.print_status = PRINT_STATUS_CHECKED_IN;
                else
                    mysql_commands.print_status = PRINT_STATUS_PRE_PRINT;

                is_updated[PRINT_STATUS] = 1;

            }

        }
        if( (str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok!="(NULL)") || (str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok!="(null)") || (str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok!="NULL"))
        {
            char* pch;
            delete[] mysql_commands.appointment_date;
            delete[] mysql_commands.appointment_time;
            pch = strtok(str_tok_buf_tmp[DB_APPOINTMENT_DATETIME].tok," ");
            mysql_commands.appointment_date = new char[strlen(pch)+1];
            strcpy(mysql_commands.appointment_date,pch);
            pch = strtok(NULL," ");
            mysql_commands.appointment_time = new char[strlen(pch)+1];
            strcpy(mysql_commands.appointment_time,pch);
            is_updated[APPOINTMENT_DATE] = 1;
            is_updated[APPOINTMENT_TIME] = 1;

        }
        if( (str_tok_buf_tmp[DB_APPOINTMENT_LOCATION].tok!="(NULL)") || (str_tok_buf_tmp[DB_APPOINTMENT_LOCATION].tok!="(null)")|| (str_tok_buf_tmp[DB_APPOINTMENT_LOCATION].tok!="NULL") )
        {
            delete[] mysql_commands.appointment_location;
            mysql_commands.appointment_location = new char[strlen(str_tok_buf_tmp[2].tok)+1];
            strcpy(mysql_commands.appointment_location,str_tok_buf_tmp[2].tok);
            is_updated[APPOINTMENT_LOCATION] = 1;

        }
        if( (str_tok_buf_tmp[DB_VISIT_ID].tok!="(NULL)") || (str_tok_buf_tmp[DB_VISIT_ID].tok!="(null)")|| (str_tok_buf_tmp[DB_VISIT_ID].tok!="NULL") )
        {
            delete[] visit_id;
            visit_id = new char[strlen(str_tok_buf_tmp[DB_VISIT_ID].tok)+1];
            strcpy(visit_id,str_tok_buf_tmp[DB_VISIT_ID].tok);

        }
    }
    else
    {
        if( (str_tok_buf_tmp[BASE_DESTINATION_X].tok!="(NULL)") || (str_tok_buf_tmp[BASE_DESTINATION_X].tok!="(null)") || (str_tok_buf_tmp[DB_HOST].tok!="NULL"))
        {
            mysql_commands.base_destination_x = atof(str_tok_buf_tmp[BASE_DESTINATION_X].tok);
            is_updated[BASE_DESTINATION_X] = 1;
        }

        if( (str_tok_buf_tmp[BASE_DESTINATION_Y].tok!="(NULL)") || (str_tok_buf_tmp[BASE_DESTINATION_Y].tok!="(null)") || (str_tok_buf_tmp[DB_HOST].tok!="NULL") )
        {
            mysql_commands.base_destination_y = atof(str_tok_buf_tmp[BASE_DESTINATION_Y].tok);
            is_updated[BASE_DESTINATION_Y] = 1;
        }

        if( (str_tok_buf_tmp[UPDATED].tok!="(NULL)") || (str_tok_buf_tmp[UPDATED].tok!="(null)") || (str_tok_buf_tmp[DB_HOST].tok!="NULL"))
        {
            strcpy(mysql_commands.updated, str_tok_buf_tmp[UPDATED].tok);
            is_updated[UPDATED] = 1;
        }

        update_master_commands(mysql_commands, is_updated);
    }

}

