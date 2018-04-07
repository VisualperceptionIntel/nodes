#include "server_node.h"


void update_master_commands(struct command_unit updated_values ,char is_updated[COMMAND_UNIT_LENGTH])
{
    char key[]="0123456789\\|<>;:,./?'@#~[]{}=+-_*&^%$£!¬`€'\"";
    char key2[]="\\|<>;/?'@#~[]{}=+_*&^%$£!¬`€'\"";
    if(master_commands.updated < updated_values.updated)
    {
        time_stamp(master_commands.updated);
        printf("\n\nUpdate Received @ %s\nFields Updated:\n",master_commands.updated);

        if( is_updated[IS_BASE_BUSY]==1 )
        {
            if( updated_values.is_base_busy == 0 || updated_values.is_base_busy==1 )
            {

                master_commands.is_base_busy = updated_values.is_base_busy;
                printf("is_base_busy value: %i\n",master_commands.is_base_busy);
                char buff[5];
                sprintf(buff,"%i",updated_values.is_base_busy);
                wizo_log(0,master_commands.updated,LOG_OK,"base",buff,0.4);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for is_base_busy field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.is_base_busy);
            }
        }
        if( is_updated[BASE_POSITION_X]==1 )
        {
            if( updated_values.base_position_x>-2000.00 && updated_values.base_position_x<2000.00 )
            {
                master_commands.base_position_x = updated_values.base_position_x;
                printf("base_position_x value: %f\n",master_commands.base_position_x);
                char buff[10];
                sprintf(buff,"%f",updated_values.base_position_x);
                wizo_log(0,master_commands.updated,LOG_OK,"base",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_position_x field. Expected between -2000.00 and 2000.00. Received: %f\n\n",
                       updated_values.base_position_x);
            }
        }
        if( is_updated[BASE_POSITION_Y]==1 )
        {
            if( updated_values.base_position_y>-2000.00 && updated_values.base_position_y<2000.00 )
            {
                master_commands.base_position_y = updated_values.base_position_y;
                printf("base_position_y value: %f\n",master_commands.base_position_y);
                char buff[10];
                sprintf(buff,"%f",updated_values.base_position_y);
                wizo_log(0,master_commands.updated,LOG_OK,"base",buff,0.2);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_position_y field. Expected between -2000.00 and 2000.00. Received: %f\n\n",
                       updated_values.base_position_y);
            }
        }
        if( is_updated[BASE_DESTINATION_X]==1 )
        {
            if( updated_values.base_destination_x>-2000.00 && updated_values.base_destination_x<2000.00 )
            {
                master_commands.base_destination_x = updated_values.base_destination_x;
                printf("base_destination_x value: %f\n",master_commands.base_destination_x);

                char buff[10];
                sprintf(buff,"%f",updated_values.base_destination_x);
                wizo_log(0,master_commands.updated,LOG_OK,"MySQL retrieval BASE_DESTINATION_X",buff,0.1);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_destination_x field. Expected between -2000.00 and 2000.00. Received: %f\n\n",
                       updated_values.base_destination_x);
            }
        }
        if( is_updated[BASE_DESTINATION_Y]==1 )
        {
            if( updated_values.base_destination_y>-2000.00 && updated_values.base_destination_y<2000.00 )
            {
                master_commands.base_destination_y = updated_values.base_destination_y;
                printf("base_destination_y value: %f\n",master_commands.base_destination_y);

                char buff[10];
                sprintf(buff,"%f",updated_values.base_destination_y);
                wizo_log(0,master_commands.updated,LOG_OK,"MySQL retrieval BASE_DESTINATION_Y",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_destination_y field. Expected between -2000.00 and 2000.00. Received: %f\n\n",
                       updated_values.base_destination_y);
            }
        }
        if ( is_updated[BASE_FORWARD_STEP]==1 )
        {
            if( updated_values.base_forward_step<11 )
            {
                master_commands.base_forward_step = updated_values.base_forward_step;
                printf("base_forward_step value: %i\n",master_commands.base_forward_step);

                char buff[5];
                sprintf(buff,"%i",updated_values.base_forward_step);
                wizo_log(0,master_commands.updated,LOG_OK,"BASE_FORWARD_STEP",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_forward_step field. Expected between 0 and 10. Received: %i\n\n",
                       updated_values.base_forward_step);
            }
        }
        if( is_updated[BASE_BACKWARD_STEP]==1 )
        {
            if( updated_values.base_backward_step<11 )
            {
                master_commands.base_backward_step = updated_values.base_backward_step;
                printf("base_backward_step value: %i\n",master_commands.base_backward_step);

                char buff[5];
                sprintf(buff,"%i",updated_values.base_backward_step);
                wizo_log(0,master_commands.updated,LOG_OK,"BASE_BACKWARD_STEP",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_backward_step field. Expected between 0 and 10. Received: %i\n\n",
                       updated_values.base_backward_step);
            }
        }
        if( is_updated[BASE_ROTATE_LEFT]==1 )
        {
            if( updated_values.base_rotate_left<11 )
            {
                master_commands.base_rotate_left = updated_values.base_rotate_left;
                printf("base_rotate_left value: %i\n",master_commands.base_rotate_left);

                char buff[5];
                sprintf(buff,"%i",updated_values.base_rotate_left);
                wizo_log(0,master_commands.updated,LOG_OK,"BASE_ROTATE_LEFT",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_rotate_left field. Expected between 0 and 10. Received: %i\n\n",
                       updated_values.base_rotate_left);
            }
        }
        if( is_updated[BASE_ROTATE_RIGHT]==1 )
        {
            if( updated_values.base_rotate_right<11 )
            {
                master_commands.base_rotate_right = updated_values.base_rotate_right;
                printf("base_rotate_right value: %i\n",master_commands.base_rotate_right);

                char buff[5];
                sprintf(buff,"%i",updated_values.base_rotate_right);
                wizo_log(0,master_commands.updated,LOG_OK,"BASE_ROTATE_RIGHT",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for base_rotate_right field. Expected between 0 and 10. Received: %i\n\n",
                       updated_values.base_rotate_right);
            }
        }
        if( is_updated[IS_LEFT_ARM_BUSY]==1 )
        {
            if( updated_values.is_left_arm_busy >= 0 || updated_values.is_left_arm_busy <=100 )
            {
                master_commands.is_left_arm_busy = updated_values.is_left_arm_busy;
                printf("is_left_arm_busy value: %i\n",master_commands.is_left_arm_busy);

                char buff[5];
                sprintf(buff,"%i",updated_values.is_left_arm_busy);
                wizo_log(0,master_commands.updated,LOG_OK,"IS_LEFT_ARM_BUSY",buff,0.1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for is_left_arm_busy field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.is_left_arm_busy);
            }
        }
        if( is_updated[IS_RIGHT_ARM_BUSY]==1 )
        {
            if( updated_values.is_right_arm_busy >= 0 || updated_values.is_right_arm_busy <=100 )
            {
                master_commands.is_right_arm_busy = updated_values.is_right_arm_busy;
                printf("is_right_arm_busy value: %i\n",master_commands.is_right_arm_busy);

                char buff[5];
                sprintf(buff,"%i",updated_values.is_right_arm_busy);
                wizo_log(0,master_commands.updated,LOG_OK,"IS_RIGHT_ARM_BUSY",buff,0.1);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for is_right_arm_busy field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.is_right_arm_busy);
            }
        }
        if( is_updated[LEFT_ARM_MOVE]==1 )
        {
            if( updated_values.left_arm_move >= 0 || updated_values.left_arm_move<=100 )
            {
                master_commands.left_arm_move = updated_values.left_arm_move;
                printf("left_arm_move value: %i\n",master_commands.left_arm_move);

                char buff[5];
                sprintf(buff,"%i",updated_values.left_arm_move);
                wizo_log(0,master_commands.updated,LOG_OK,"LEFT_ARM_MOVE",buff,0.2);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for left_arm_move field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.left_arm_move);
            }
        }
        if( is_updated[RIGHT_ARM_MOVE]==1 )
        {
            if( updated_values.right_arm_move >= 0 || updated_values.right_arm_move<=100 )
            {
                master_commands.right_arm_move = updated_values.right_arm_move;
                printf("right_arm_move value: %i\n",master_commands.right_arm_move);

                char buff[5];
                sprintf(buff,"%i",updated_values.right_arm_move);
                wizo_log(0,master_commands.updated,LOG_OK,"RIGHT_ARM_MOVE",buff,0.1);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for right_arm_move field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.right_arm_move);
            }
        }
        if( is_updated[EMOTION_STATUS]==1 )
        {
            if( updated_values.emotion_status<7 )
            {
                master_commands.emotion_status = updated_values.emotion_status;
                printf("emotion_status value: %i\n",master_commands.emotion_status);
                emotions_count[updated_values.emotion_status] +=1;

                char buff[5];
                sprintf(buff,"%i",updated_values.emotion_status);
                wizo_log(0,master_commands.updated,LOG_OK,"EMOTION_STATUS",buff,0.2);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for emotion_status field. Expected between 0 and 6. Received: %i\n\n",
                       updated_values.emotion_status);
            }
        }
        if( is_updated[LIGHT_STATUS]==1 )
        {
            if( updated_values.light_status > 6 && updated_values.light_status < 10 )
            {
                master_commands.light_status = updated_values.light_status;
                printf("light_status value: %i\n",master_commands.light_status);

                char buff[5];
                sprintf(buff,"%i",updated_values.light_status);
                wizo_log(0,master_commands.updated,LOG_OK,"LIGHT_STATUS",buff,4);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for light_status field. Expected between 7 and 9. Received: %i\n\n",
                       updated_values.light_status);
            }
        }
        if( is_updated[PRINT_STATUS]==1 )
        {
            if( updated_values.print_status == 0 || updated_values.print_status==1 )
            {
                master_commands.print_status = updated_values.print_status;
                printf("print_status value: %i\n",master_commands.print_status);

                char buff[5];
                sprintf(buff,"%i",updated_values.print_status);
                wizo_log(0,master_commands.updated,LOG_OK,"PRINT_STATUS",buff,4);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for print_status field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.print_status);
            }
        }
        if( is_updated[FACE_COGNITION_STATUS]==1 )
        {
            if( updated_values.face_cognition_status == 0 || updated_values.face_cognition_status==1 )
            {
                master_commands.face_cognition_status = updated_values.face_cognition_status;
                printf("face_cognition_status value: %i\n",master_commands.face_cognition_status);

                char buff[5];
                sprintf(buff,"%i",updated_values.face_cognition_status);
                wizo_log(0,master_commands.updated,LOG_OK,"FACE_COGNITION_STATUS",buff,6);
            }
            else
            {
                cout << "value not valid" << endl;
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for face_cognition_status field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.face_cognition_status);
            }
        }
        if( is_updated[CARD_READER_STATUS]==1 )
        {
            if( updated_values.card_reader_status == 0 || updated_values.card_reader_status==1 )
            {
                master_commands.card_reader_status = updated_values.card_reader_status;
                printf("card_reader_status value: %i\n",master_commands.card_reader_status);

                char buff[5];
                sprintf(buff,"%i",updated_values.card_reader_status);
                wizo_log(0,master_commands.updated,LOG_OK,"CARD_READER_STATUS",buff,1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for card_reader_status field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.card_reader_status);
            }
        }
        if( is_updated[FACE_COGNITION_OUTCOME]==1 )
        {
            if( strpbrk(updated_values.face_cognition_outcome,key)==NULL )
            {
                if( master_commands.face_cognition_outcome != NULL)
                {
                    free(master_commands.face_cognition_outcome);
                    master_commands.face_cognition_outcome = new char[strlen(updated_values.face_cognition_outcome)+1];
                }
                else
                    master_commands.face_cognition_outcome = new char[strlen(updated_values.face_cognition_outcome)+1];
                strcpy(master_commands.face_cognition_outcome, updated_values.face_cognition_outcome);
                printf("face_cognition_outcome value: %s\n",master_commands.face_cognition_outcome);

                //char buff[5];
                //sprintf(buff,"%i",updated_values.card_reader_status);
                wizo_log(0,master_commands.updated,LOG_OK,"FACE_COGNITION_OUTCOME",updated_values.face_cognition_outcome,1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in face_cognition_outcome field. Received: %s\nName can't contain %s.\n\n",
                       updated_values.face_cognition_outcome,key);
            }
        }
        if( is_updated[ID_CARD_NAME]==1 )
        {
            if( strpbrk(updated_values.id_card_name,key)==NULL )
            {
                if( master_commands.id_card_name != NULL)
                {
                    free(master_commands.id_card_name);
                    master_commands.id_card_name = new char[strlen(updated_values.id_card_name)+1];
                }
                else
                    master_commands.id_card_name = new char[strlen(updated_values.id_card_name)+1];
                strcpy(master_commands.id_card_name, updated_values.id_card_name);
                printf("id_card_name value: %s\n",master_commands.id_card_name);
                wizo_log(0,master_commands.updated,LOG_OK,"ID_CARD_NAME",updated_values.id_card_name,1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in id_card_name field. Received: %s\nName can't contain %s.\n",
                       updated_values.id_card_name,key);
                wizo_log(1,master_commands.updated,LOG_ERROR,"ID_CARD_NAME",updated_values.id_card_name,5);
            }
        }
        if( is_updated[VISITEE_NAME]==1 )
        {
            if( strpbrk(updated_values.visitee_name,key)==NULL )
            {
                if( master_commands.visitee_name != NULL)
                {
                    free(master_commands.visitee_name);
                    master_commands.visitee_name = new char[strlen(updated_values.visitee_name)+1];
                }
                else
                    master_commands.visitee_name = new char[strlen(updated_values.visitee_name)+1];
                strcpy(master_commands.visitee_name, updated_values.visitee_name);
                printf("visitee_name value: %s\n",master_commands.visitee_name);
                wizo_log(0,master_commands.updated,LOG_OK,"MySQL retrieval VISITEE_NAME",updated_values.visitee_name,4);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in visitee_name field. Received: %s\nName can't contain %s.\n",
                       updated_values.visitee_name,key);
            }
        }
        if( is_updated[APPOINTMENT_DATE]==1 )
        {
            if( strpbrk(updated_values.appointment_date,key2)==NULL )
            {
                if( master_commands.appointment_date != NULL)
                {
                    free(master_commands.appointment_date);
                    master_commands.appointment_date = new char[strlen(updated_values.appointment_date)+1];
                }
                else
                    master_commands.appointment_date = new char[strlen(updated_values.appointment_date)+1];
                strcpy(master_commands.appointment_date, updated_values.appointment_date);
                printf("appointment_date value: %s\n",master_commands.appointment_date);
                wizo_log(0,master_commands.updated,LOG_OK,"MySQL retrieval APPOINTMENT_DATE",updated_values.appointment_date,2);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in appointment_date field. Received: %s\nName can't contain %s.\n",
                       updated_values.appointment_date,key2);
            }
        }
        if( is_updated[APPOINTMENT_TIME]==1 )
        {
            if( strpbrk(updated_values.appointment_time,key2)==NULL )
            {
                if( master_commands.appointment_time != NULL)
                {
                    free(master_commands.appointment_time);
                    master_commands.appointment_time = new char[strlen(updated_values.appointment_time)+1];
                }
                else
                    master_commands.appointment_time = new char[strlen(updated_values.appointment_time)+1];
                strcpy(master_commands.appointment_time, updated_values.appointment_time);
                printf("appointment_time value: %s\n",master_commands.appointment_time);
                wizo_log(0,master_commands.updated,LOG_OK,"MySQL retrieval APPOINTMENT_TIME",updated_values.appointment_time,4);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in appointment_time field. Received: %s\nName can't contain %s.\n",
                       updated_values.appointment_time,key2);
            }
        }
        if( is_updated[APPOINTMENT_LOCATION]==1 )
        {
            if( strpbrk(updated_values.appointment_location,key2)==NULL )
            {
                if( master_commands.appointment_location != NULL)
                {
                    free(master_commands.appointment_location);
                    master_commands.appointment_location = new char[strlen(updated_values.appointment_location)+1];
                }
                else
                    master_commands.appointment_location = new char[strlen(updated_values.appointment_location)+1];
                strcpy(master_commands.appointment_location, updated_values.appointment_location);
                printf("appointment_location value: %s\n",master_commands.appointment_location);
                wizo_log(0,master_commands.updated,LOG_OK,"MySQL retrieval APPOINTMENT_LOCATION",updated_values.appointment_location,5);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in appointment_location field. Received: %s\nName can't contain %s.\n",
                       updated_values.appointment_location,key2);
            }
        }
        if( is_updated[SPEECH_REQUEST]==1 )
        {
            if( updated_values.speech_request == 0 || updated_values.speech_request==1 )
            {
                master_commands.speech_request = updated_values.speech_request;
                printf("speech_request value: %i\n",master_commands.speech_request);

                char buff[5];
                sprintf(buff,"%i",updated_values.speech_request);
                wizo_log(0,master_commands.updated,LOG_OK,"SPEECH_REQUEST",buff,3);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for speech_request field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.speech_request);
            }
        }
        if( is_updated[SPEECH_STATUS]==1 )
        {
            if( updated_values.speech_status == 0 || updated_values.speech_status==1 )
            {
                master_commands.speech_status = updated_values.speech_status;
                printf("speech_status value: %i\n",master_commands.speech_status);

                char buff[10];
                sprintf(buff,"%i",updated_values.speech_status);
                wizo_log(0,master_commands.updated,LOG_OK,"SPEECH_STATUS",buff,4);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for speech_status field. Expected between 0 and 1. Received: %i\n\n",
                       updated_values.speech_status);
            }
        }
        //********************************************************************
        if( is_updated[ARM_ACTIONS]==1 )
        {
            if( strpbrk(updated_values.arm_actions,key2)==NULL )
            {
                if( master_commands.arm_actions != NULL)
                {
                    free(master_commands.arm_actions);
                    master_commands.arm_actions = new char[strlen(updated_values.arm_actions)+1];
                }
                else
                    master_commands.arm_actions = new char[strlen(updated_values.arm_actions)+1];
                strcpy(master_commands.arm_actions, updated_values.arm_actions);
                printf("arm_actions value: %s\n",master_commands.arm_actions);


                wizo_log(0,master_commands.updated,LOG_OK,"ARM_ACTIONS",master_commands.arm_actions,4);

            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in arm_actions field. Received: %s\nName can't contain %s.\n",
                       updated_values.arm_actions,key2);
            }
        }


        if( is_updated[CURRENT_LATITUDE]==1 )
        {
            if( updated_values.current_latitude > -90 || updated_values.current_latitude < 90 )
            {
                master_commands.current_latitude = updated_values.current_latitude;
                printf("current_latitude value: %f\n",master_commands.current_latitude);

                char buff[10];
                sprintf(buff,"%f",updated_values.current_latitude);
                wizo_log(0,master_commands.updated,LOG_OK,"CURRENT_LATITUDE",buff,7);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for current_latitude field. Expected between -90 and 90. Received: %f\n\n",
                       updated_values.current_latitude);
            }
        }

        if( is_updated[CURRENT_LONGITUDE]==1 )
        {
            if( updated_values.current_longitude > -180 || updated_values.current_longitude < 180 )
            {
                master_commands.current_longitude = updated_values.current_longitude;
                printf("current_longitude value: %f\n",master_commands.current_longitude);

                char buff[10];
                sprintf(buff,"%f",updated_values.current_longitude);
                wizo_log(0,master_commands.updated,LOG_OK,"CURRENT_LONGITUDE",buff,5);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for current_longitude field. Expected between -180 and 180. Received: %f\n\n",
                       updated_values.current_longitude);
            }
        }

        if( is_updated[CURRENT_ALTITUDE]==1 )
        {
            if( updated_values.current_altitude >  -180 || updated_values.current_altitude < 180 )
            {
                master_commands.current_altitude = updated_values.current_altitude;
                printf("current_altitude value: %f\n",master_commands.current_altitude);

                char buff[10];
                sprintf(buff,"%f",updated_values.current_altitude);
                wizo_log(0,master_commands.updated,LOG_OK,"CURRENT_ALTITUDE",buff,5);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for current_altitude field. Expected between -180 and 180. Received: %f\n\n",
                       updated_values.current_altitude);
            }
        }
        if( is_updated[OBJ_FRAME]==1 )
        {
            if( strpbrk(updated_values.obj_frame,key2)==NULL )
            {
                if( master_commands.obj_frame != NULL)
                {
                    free(master_commands.obj_frame);
                    master_commands.obj_frame = new char[strlen(updated_values.obj_frame)+1];
                }
                else
                    master_commands.obj_frame = new char[strlen(updated_values.obj_frame)+1];
                strcpy(master_commands.obj_frame, updated_values.obj_frame);
                printf("obj_frame value: %s\n",master_commands.obj_frame);
                wizo_log(0,master_commands.updated,LOG_OK,"OBJ_FRAME",master_commands.obj_frame,3);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nSpecial characters detected in obj_frame field. Received: %s\nName can't contain %s.\n",
                       updated_values.obj_frame,key2);
            }
        }
        if( is_updated[OBJ_X]==1 )
        {
            if( updated_values.obj_x >  -1000 || updated_values.obj_x < 1000 )
            {
                master_commands.obj_x = updated_values.obj_x;
                printf("obj_x value: %f\n",master_commands.obj_x);
                char buff[10];
                sprintf(buff,"%f",updated_values.obj_x);
                wizo_log(0,master_commands.updated,LOG_OK,"OBJ_X",buff,5);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for obj_x field. Expected between -1000 and 1000. Received: %f\n\n",
                       updated_values.obj_x);
            }
        }
        if( is_updated[OBJ_Y]==1 )
        {
            if( updated_values.obj_y >  -1000 || updated_values.obj_y < 1000 )
            {
                master_commands.obj_y = updated_values.obj_y;
                printf("obj_y value: %f\n",master_commands.obj_y);
                char buff[10];
                sprintf(buff,"%f",updated_values.obj_y);
                wizo_log(0,master_commands.updated,LOG_OK,"OBJ_Y",buff,2);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for obj_y field. Expected between -1000 and 1000. Received: %f\n\n",
                       updated_values.obj_y);
            }
        }
        if( is_updated[OBJ_Z]==1 )
        {
            if( updated_values.obj_z >  -1000 || updated_values.obj_z < 1000 )
            {
                master_commands.obj_z = updated_values.obj_z;
                printf("obj_z value: %f\n",master_commands.obj_z);
                char buff[10];
                sprintf(buff,"%f",updated_values.obj_z);
                wizo_log(0,master_commands.updated,LOG_OK,"OBJ_Z",buff,1);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for obj_z field. Expected between -1000 and 1000. Received: %f\n\n",
                       updated_values.obj_z);
            }
        }
        if( is_updated[OBJ_HEIGHT]==1 )
        {
            if( updated_values.obj_height >  0 || updated_values.current_altitude < 1 )
            {
                master_commands.obj_height = updated_values.obj_height;
                printf("obj_height value: %f\n",master_commands.obj_height);
                char buff[10];
                sprintf(buff,"%f",updated_values.obj_height);
                wizo_log(0,master_commands.updated,LOG_OK,"OBJ_HEIGHT",buff,0.2);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for obj_height field. Expected between 0 and 1. Received: %f\n\n",
                       updated_values.obj_height);
            }
        }
        if( is_updated[OBJ_WIDTH]==1 )
        {
            if( updated_values.obj_width >  0 || updated_values.obj_width < 1 )
            {
                master_commands.obj_width = updated_values.obj_width;
                printf("obj_width value: %f\n",master_commands.obj_width);
                char buff[10];
                sprintf(buff,"%f",updated_values.obj_width);
                wizo_log(0,master_commands.updated,LOG_OK,"OBJ_WIDTH",buff,.2);
            }
            else
            {
                printf("\nWARNING: Field NOT updated. \nOut of range value dectected for obj_width field. Expected between 0 and 1. Received: %f\n\n",
                       updated_values.obj_width);
            }
        }


    }
    else
    {
        printf("\nWARNING: Requested update time stamped in the past. \nLast Master Update: %s, Time Stamp of Update: %s\n",
               master_commands.updated,updated_values.updated);
    }

    cout << "Update Completed\n" << endl;
    //exit(0);
}
