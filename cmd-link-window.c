/* $Id$ */

/*
 * Copyright (c) 2007 Nicholas Marriott <nicm@users.sourceforge.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>

#include <stdlib.h>

#include "tmux.h"

/*
 * Link a window into another session.
 */

enum cmd_retval	 cmd_link_window_exec(struct cmd *, struct cmd_q *);

const struct cmd_entry cmd_link_window_entry = {
	"link-window", "linkw",
	"dks:t:", 0, 0,
	"[-dk] " CMD_SRCDST_WINDOW_USAGE,
	0,
	NULL,
	cmd_link_window_exec
};

enum cmd_retval
cmd_link_window_exec(struct cmd *self, struct cmd_q *cmdq)
{
	struct args	*args = self->args;
	struct session	*src, *dst;
	struct winlink	*wl;
	char		*cause;
	int		 idx, kflag, dflag;

	if ((wl = cmd_find_window(cmdq, args_get(args, 's'), &src)) == NULL)
		return (CMD_RETURN_ERROR);
	if ((idx = cmd_find_index(cmdq, args_get(args, 't'), &dst)) == -2)
		return (CMD_RETURN_ERROR);

	kflag = args_has(self->args, 'k');
	dflag = args_has(self->args, 'd');
	if (server_link_window(src, wl, dst, idx, kflag, !dflag, &cause) != 0) {
		cmdq_error(cmdq, "can't link window: %s", cause);
		free(cause);
		return (CMD_RETURN_ERROR);
	}
	recalculate_sizes();

	return (CMD_RETURN_NORMAL);
}
