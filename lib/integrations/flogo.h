#include <linux/fs.h>

const char* FLOGO = 
"                        ###                                        ###   ###                       \n"
"                       #####    ########################           #### #####                      \n"
"                      ##   ##  ####################################### ##   ##                     \n"
"                      ##   ## ###            ########################  ##   ##                     \n"
"                      ##   ## ####                                     ##   ##                     \n"
"                       #####   ##              ######                   #####                      \n"
"                        ###                  ##########                  ###                       \n"
"                                           ####      ####              #                           \n"
"                         ###              ##            ##           ####                          \n"
"                        #####            ##              ##          ####                          \n"
"                         ## ##          ##                ##         ####                          \n"
"                         ## ###        ##                  ##          ##                          \n"
"                             ##        ##                  ##         ##                           \n"
"                             ###      ##                    ##        ##                           \n"
"                              ###     ##                    ##       ##                            \n"
"                              ###     #                     ##      ###                            \n"
"                               ###    #                      #      ##                             \n"
"                               ###   ##                      ##    ###                             \n"
"                                ###   #                      #    ###                              \n"
"                                ###   #                      #    ###                              \n"
"                                 ###  ##                    ##   ###                               \n"
"                                 ###  ##                    ##  ###                                \n"
"                                  ###  #                    #   ###                                \n"
"                                  ###  ##                  ##  ###                                 \n"
"                                   ###  ##                ##  ###                                  \n"
"                                   ###   #                #   ###                                  \n"
"                                    ###   ##            ##   ###                                   \n"
"                                    ###    ###        ###   ####                                   \n"
"                                     ###    ############    ###                                    \n"
"                                     ###      ########     ###                                     \n"
"                                      ###                 ####                                     \n"
"                                      ###                 ###                                      \n"
"                                       ###               ###                                       \n"
"                                       ###               ###                                       \n"
"                                        ###             ###                                        \n"
"                                        ###            ###                                         \n"
"                                         ###           ###                                         \n"
"                                          ##          ###                                          \n"
"                                          ###  #      ##                                           \n"
"                                           ## ###    ##                                            \n"
"                                            #####    ##                                            \n"
"                                             ###     ####                                          \n"
"                                                     ####                                          \n"
"                                                #### ###                                           \n"
"                                               ######                                              \n"
"                                               #    #                                              \n"
"                                               #    #                                              \n"
"                                               ##  ##                                              \n"
"                                                ####                                               \n"
"                                                 ##                                                ";

static ssize_t flogo_r(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    int len = strlen(FLOGO); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, FLOGO, len)) { 
        pr_info("we finished displaying the flogo\n"); 
        ret = 0; 
    } else { 
        pr_info("reading flogo... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
} 
