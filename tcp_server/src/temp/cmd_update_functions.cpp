#include "server_node.h"

int base_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"base_busy")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.is_base_busy=atoi(str_tok_buf_tmp[3].tok);
                is_updated[IS_BASE_BUSY] = 1;


            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"pos_x")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
            {
                command_update.base_position_x=atof(str_tok_buf_tmp[5].tok);
                is_updated[BASE_POSITION_X] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"pos_y")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0 )
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
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0 )
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
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.is_left_arm_busy=atoi(str_tok_buf_tmp[3].tok);
                is_updated[IS_LEFT_ARM_BUSY] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"right_busy")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
            {
                command_update.is_right_arm_busy=atoi(str_tok_buf_tmp[5].tok);
                is_updated[IS_RIGHT_ARM_BUSY] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"arm_actions")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0 )
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

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"emotion")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.emotion_status=atoi(str_tok_buf_tmp[3].tok);
                is_updated[EMOTION_STATUS] = 1;

            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"cog_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
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
                mysql_visit_update(sql_visit_tok_buf,is_updated,command_update ,2 );//1-scanner 2-cognition
            }
        }
        //###################################################
        //should be changed to face_detect_status
        //###################################################
        if( strcmp(str_tok_buf_tmp[8].tok,"speech_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0 )
            {
                command_update.speech_status=atoi(str_tok_buf_tmp[9].tok);
                is_updated[SPEECH_STATUS] = 1;
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

int scanner_cmd_update(struct command_unit& command_update, str_tok_buffer& str_tok_buf_tmp)
{
    char is_updated[COMMAND_UNIT_LENGTH] = {};

    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        // Reset command structure
        command_update = command_unit();

        if( strcmp(str_tok_buf_tmp[2].tok,"id_card_name")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.id_card_name=str_tok_buf_tmp[3].tok;

                //log_string=log_string+","+str_tok_buf_tmp[2].tok+":"+str_tok_buf_tmp[3].tok;

                is_updated[ID_CARD_NAME] = 1;

                str_tok_buffer sql_visit_tok_buf = {NULL};
                mysql_visit_update(sql_visit_tok_buf,is_updated,command_update,1 );//1-scanner 2-cognition
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"card_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
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
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.print_status=atoi(str_tok_buf_tmp[3].tok);
                is_updated[PRINT_STATUS] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"card_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
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
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.base_forward_step=atoi(str_tok_buf_tmp[3].tok);
                is_updated[BASE_FORWARD_STEP] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"backward_step")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
            {
                command_update.base_backward_step=atoi(str_tok_buf_tmp[5].tok);
                is_updated[BASE_BACKWARD_STEP] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"rotate_left")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0 )
            {
                command_update.base_rotate_left=atoi(str_tok_buf_tmp[7].tok);
                is_updated[BASE_ROTATE_LEFT] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[8].tok,"rotate_right")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0 )
            {
                command_update.base_rotate_right=atoi(str_tok_buf_tmp[9].tok);
                is_updated[BASE_ROTATE_RIGHT] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[10].tok,"light_status")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[11].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[11].tok,"(null)")!=0 )
            {
                command_update.light_status=atoi(str_tok_buf_tmp[11].tok);
                is_updated[LIGHT_STATUS] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[12].tok,"latitude")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[13].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[13].tok,"(null)")!=0 )
            {
                command_update.current_latitude=atof(str_tok_buf_tmp[13].tok);
                is_updated[CURRENT_LATITUDE] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[14].tok,"longitude")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[15].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[15].tok,"(null)")!=0 )
            {
                command_update.current_longitude=atof(str_tok_buf_tmp[15].tok);
                is_updated[CURRENT_LONGITUDE] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[16].tok,"altitude")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[17].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[17].tok,"(null)")!=0 )
            {
                command_update.current_altitude=atof(str_tok_buf_tmp[17].tok);
                is_updated[CURRENT_ALTITUDE] = 1;
            }
        }


        if( strcmp(str_tok_buf_tmp[18].tok,"welcome")==0 )
        {

            ret = 2;

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
            if ( strcmp(str_tok_buf_tmp[3].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[3].tok,"(null)")!=0 )
            {
                command_update.obj_frame=str_tok_buf_tmp[3].tok;
                is_updated[OBJ_FRAME] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[4].tok,"obj_x")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[5].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[5].tok,"(null)")!=0 )
            {
                command_update.obj_x=atof(str_tok_buf_tmp[5].tok);
                is_updated[OBJ_X] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[6].tok,"obj_y")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[7].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[7].tok,"(null)")!=0 )
            {
                command_update.obj_y=atof(str_tok_buf_tmp[7].tok);
                is_updated[OBJ_Y] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[8].tok,"obj_z")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[9].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[9].tok,"(null)")!=0 )
            {
                command_update.obj_z=atof(str_tok_buf_tmp[9].tok);
                is_updated[OBJ_Z] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[10].tok,"obj_height")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[11].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[11].tok,"(null)")!=0 )
            {
                command_update.obj_height=atof(str_tok_buf_tmp[11].tok);
                is_updated[OBJ_HEIGHT] = 1;
            }
        }
        if( strcmp(str_tok_buf_tmp[12].tok,"obj_width")==0 )
        {
            if ( strcmp(str_tok_buf_tmp[13].tok,"(NULL)")!=0 && strcmp(str_tok_buf_tmp[13].tok,"(null)")!=0 )
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


int lights_cmd_update(str_tok_buffer& str_tok_buf_tmp)
{
    if( strcmp(str_tok_buf_tmp[1].tok,"update")==0 )
    {
        return 1;
    }
    return -1;
}
