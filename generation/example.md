# Example the command to generate events

Here the example command is presented.
One need to take an extra attension to the beam position x and y, as well as a sigma indicated as `--beam_x`(`--beam_y`) and `--beam_sigma_x`(`--beam_sigma_y`).
These values are determined by the Gaussian fit to the reconstructed fit distribution, which was explain in the [talk](https://agenda.linearcollider.org/event/10098/contributions/53072/attachments/39111/61634/CM_09.pdf) at the CALICE meeting 2023 at FZU.

```shell
./send_condor.py --particle e- --energy 10 --beam_x -53.43 --beam_y -41.73 --beam_sigma_x 23.5 --beam_sigma_y 29.7 --nevents 5000 --tbconf TB2022-06_CONF6
```