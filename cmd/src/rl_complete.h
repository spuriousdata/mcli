#ifndef __RL_COMPLETE_H__
#define __RL_COMPLETE_H__

#ifdef __cplusplus
extern "C" {
#endif

char **mci_completion(const char *text, int start, int end);
char **history_matches(const char *text, int start);
char *command_generator(const char *text, int state);
void initialize_readline();

#ifdef __cplusplus
}
#endif

#endif
