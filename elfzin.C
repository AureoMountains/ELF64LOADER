
uint16_t findMode(int *x, int *y, int *d)
{
    VbeInfoBlock *ctrl = (VbeInfoBlock *)0x2000;
    ModeInfoBlock *inf = (ModeInfoBlock *)0x3000;

    uint16_t *modes;
    uint16_t best = 0x13;

    int bestpixdiff = DIFF(320 * 200, *x * *y);
    int bestdepthdiff = (*d <= 8) ? 8 - *d : (*d - 8) * 2;

    strncpy(ctrl->VbeSignature, "VBE2", 4);

    intV86(0x10, "ax,es:di", 0x4F00, 0, ctrl);

    if ((uint16_t)v86.tss.eax != 0x004F)
        return best;

    modes = (uint16_t *)REALPTR(ctrl->VideoModePtr);

    for (int i = 0; modes[i] != 0xFFFF; i++) {
        intV86(0x10, "ax,cx,es:di", 0x4F01, modes[i], 0, inf);

        if ((uint16_t)v86.tss.eax != 0x004F)
            continue;

        if (!(inf->attributes & 0x80))
            continue;

        if (inf->memory_model != 4 && inf->memory_model != 6)
            continue;

        if (*x == inf->XResolution &&
            *y == inf->YResolution &&
            *d == inf->BitsPerPixel)
            return modes[i];

        int pixdiff = DIFF(
            inf->XResolution * inf->YResolution,
            *x * *y
        );

        int depthdiff = (inf->BitsPerPixel >= *d)
            ? inf->BitsPerPixel - *d
            : (*d - inf->BitsPerPixel) * 2;

        if (pixdiff < bestpixdiff ||
            (pixdiff == bestpixdiff && depthdiff < bestdepthdiff)) {
            best = modes[i];
            bestpixdiff = pixdiff;
            bestdepthdiff = depthdiff;
        }
    }

    if (*x == 640 && *y == 480 && *d == 1)
        return 0x11;

    return best;
}
