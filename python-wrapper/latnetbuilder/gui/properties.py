import ipywidgets as widgets
import numpy as np

from .common import style_default, parse_polynomial, INITIAL_DIM, BaseGUIElement, trigger_display

def change_modulus(change, gui):
    if change['name'] != 'value':
        return
    try:
        poly_str = parse_polynomial(change['new'])
        if len(poly_str) > 0:
            gui.properties.modulus_pretty.layout.display = 'flex'
            gui.properties.modulus_pretty.value = '\\(' + poly_str + '\\)'
        else:
            gui.properties.modulus_pretty.layout.display = 'none'
    except:
        gui.properties.modulus_pretty.layout.display = 'none'
        return

def change_interlacing(change, gui):
    if change['name'] != 'value':
        return
    if change['new'] == 1:
        if gui.main_tab.selected_index == 0:
            gui.figure_of_merit.figure_type.options = ['Palpha', 'R']
        else:
            gui.figure_of_merit.figure_type.options = ['Palpha', 'R', 't-value', 'resolution-gap']
    else:
        gui.figure_of_merit.figure_type.options = ['IAalpha', 'IB']    
        gui.figure_of_merit.figure_type.value = 'IAalpha'
    update(gui.exploration_method.generating_vector.children[0], change['new']*gui.properties.dimension.value, '1', '100px')
    update(gui.exploration_method.generating_vector_simple, change['new']*gui.properties.dimension.value, '1', '100px')

def update(form, dim, defaut_value, width):
    new_children = []
    if len(form.children) < dim:
        for k in range(len(form.children)):
            new_children.append(form.children[k])
        for k in range(dim-len(form.children)):
            new_children.append(widgets.Text(
                value=defaut_value, description='', layout=widgets.Layout(width=width)))
    elif len(form.children) > dim:
        for k in range(dim):
            new_children.append(form.children[k])
    form.children = new_children

# callback for dimension change
def change_dimension(change, gui):
    if change['name'] != 'value':
        return
    dim = change['new']
    VBOX_of_weights = gui.weights.VBOX_of_weights
    for k in range(len(VBOX_of_weights.children)):
        weight = VBOX_of_weights.children[k]
        weight_type = weight.children[0].children[0].value.split(' ')[1]
        if weight_type == 'Product':
            form = weight.children[1].children[0].children[1]
            update(form, dim, '0.8', '50px')
        elif weight_type == 'Order-Dependent':
            new_value = str(round(0.8**dim * (dim <= 3), 3))
            form = weight.children[1].children[0].children[1]
            update(form, dim, new_value, '50px')
        elif weight_type == 'POD':
            form = weight.children[1].children[0].children[1]
            update(form, dim, '0.8', '50px')
            new_value = str(round(0.8**dim * (dim <= 3), 3))
            form = weight.children[2].children[0].children[1]
            update(form, dim, new_value, '50px')
    gui.exploration_method.mixed_CBC_level.max = dim

    dim = change['new'] * gui.properties.interlacing.value
    update(gui.exploration_method.generating_vector.children[0], dim, '1', '100px')
    update(gui.exploration_method.generating_vector_simple, dim, '1', '100px')


def properties():
    modulus = widgets.Text(placeholder='e.g. 2^10 (default) or 1024', description='Modulus n=', 
                    style=style_default, layout=widgets.Layout(width='95%'))

    is_multilevel = widgets.Checkbox(value=False, description='Multilevel')

    dimension = widgets.BoundedIntText(value=INITIAL_DIM, min=1, description='Dimension s:',
                                       style=style_default, layout=widgets.Layout(width='90%'))
    interlacing = widgets.BoundedIntText(value=1, min=1, description='Interlacing d:',
                                       style=style_default, layout=widgets.Layout(width='90%'), disabled=True)

    modulus_pretty = widgets.Label('', layout=widgets.Layout(display='none'))

    properties_wrapper = widgets.Accordion(
        [widgets.HBox(
            [widgets.VBox([modulus, modulus_pretty], layout=widgets.Layout(width='50%')), 
            is_multilevel, 
            widgets.VBox([dimension, interlacing], layout=widgets.Layout(width='20%'))],
            layout=widgets.Layout(align_items='center')
        )])
    properties_wrapper.set_title(0, 'Basic Lattice properties')
    return BaseGUIElement(modulus=modulus,
                          is_multilevel=is_multilevel,
                          dimension=dimension,
                          interlacing=interlacing,
                          main=properties_wrapper,
                          modulus_pretty=modulus_pretty,
                          _callbacks={'modulus': change_modulus,
                                      'dimension': change_dimension,
                                      'interlacing': change_interlacing,
                                      'is_multilevel': trigger_display})
