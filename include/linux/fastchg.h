/*
 * Force Fast Charge Driver Header
 * Author: Xiaomi-13T-Hype | モトテーパー
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef _LINUX_FASTCHG_H
#define _LINUX_FASTCHG_H

#ifdef CONFIG_FORCE_FAST_CHARGE
extern int force_fast_charge;
#else
#define force_fast_charge 0
#endif

#endif /* _LINUX_FASTCHG_H */
