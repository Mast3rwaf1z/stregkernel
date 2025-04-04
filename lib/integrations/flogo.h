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
"                                                 ##                                                \n";

ssize_t flogo(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset); 
